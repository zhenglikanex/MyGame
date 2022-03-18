#include "Game.hpp"

#include <chrono>

#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/InputService.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/LuaBridge.hpp"
#include "Framework/Game/Registry.hpp"

#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Local.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorUtility.hpp"

#include "Framework/Game/Data/SceneConfig.hpp"

using namespace std::chrono;

Game::Game(Locator&& locator,std::vector<PlayerInfo>&& players)
	: start_time_(0)
	, run_time_(0)
	, run_frame_(0)
	, input_frame_rate_(GameConfig::kDefaultInputFrame)
	, registry_(locator.Ref<const GameHelper>())
	, player_infos_(players)
{
	registry_.set<Locator>(std::move(locator));
}

Game::~Game()
{
	
}

bool Game::Initialize(uint32_t start_time)
{
	start_time_ = start_time;

	SetCtx();

	InitializeSyncInstances();

	RegisterLua(registry_, registry_.ctx<sol::state>());

	LoadScene("Battle.json");

	CreatePlayer();

	auto& game_helper = registry_.ctx<Locator>().Ref<const GameHelper>();
	systems_ = game_helper.CreateSystems(&registry_);

	for (auto& system : systems_)
	{
		if (!system->Initialize())
		{
			return false;
		}
	}

	return true;
}

void Game::InitializeSyncInstances()
{
	auto& game_helper = registry_.ctx<Locator>().Ref<const GameHelper>();
	auto& sync_manager = registry_.set<SyncManager>();
	for (auto& info : player_infos_)
	{
		sync_manager.sync_instances.emplace(info.id, SyncInstance(info.id, 0));
	}

	if (game_helper.IsAuthority())
	{
		//插入服务器控制syncdata
		sync_manager.sync_instances.emplace(ServerInstance, SyncInstance(ServerInstance, 0));
	}
}

void Game::SetCtx()
{
	registry_.set<GameState>();
	registry_.set<sol::state>();
	registry_.set<GameplayEffectManager>(registry_);
	registry_.set<GameplayAbilityManager>(registry_);
	registry_.set<SyncManager>();
}

void Game::UnSetCtx()
{
	registry_.unset<SyncManager>();
	registry_.unset<GameplayAbilityManager>();
	registry_.unset<GameplayEffectManager>();
	registry_.unset<sol::state>();
	registry_.unset<GameState>();
}

void Game::Update(float dt)
{
	//run_time_ += dt;
	// todo:暂时用这种方式计算时间，dt精度不够，后面再改
	uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	run_time_ = (now - start_time_) / 1000.0f;

	UpdateInput();
	UpdateLogic(dt);
	UpdateFixedLogic();
}

void Game::UpdateInput()
{
	while (run_time_ > run_frame_ * input_frame_rate_ + input_frame_rate_)
		//&& (run_frame_ - real_frame_) < GameConfig::kMaxPredictFrame)
	{
		//会触发InputCommand,相当于getcommands
		auto& input_service = registry_.ctx<Locator>().Ref<const InputService>();
		input_service.InputHandler();

		auto& group = GetCommandGroup(run_frame_);

		assert((group.commands.size() == player_infos_.size()) && "not commands");

		++run_frame_;
	}
}

void Game::UpdateLogic(float dt)
{
	for (auto& system : systems_)
	{
		system->Update(dt);
	}
}

void Game::UpdateFixedLogic()
{
	int run_frame_times = 0;
	auto& game_state = registry_.ctx<GameState>();
	while (true)
	{
		// 模拟帧数不能超过当前可以运行帧数，因为有可能提前收到服务器帧
		if (game_state.run_frame >= run_frame_)
		{
			return;
		}

		auto& command_group = GetCommandGroup(game_state.run_frame);
		if (command_group.frame != game_state.run_frame || run_frame_times >= GameConfig::kMaxOnceFrameRunNum)
		{
			return;
		}

		for (auto e : registry_.view<Player,Local>())
		{
			const auto& player = registry_.get<Player>(e);
			auto iter = command_group.commands.find(player.id);
			if (iter != command_group.commands.end())
			{
				registry_.emplace_or_replace<Command>(e, iter->second);
			}
		}

		for (auto& system : systems_)
		{
			system->FixedUpdate(GameConfig::kFrameTime);
		}

		for (auto& system : systems_)
		{
			system->LateUpdate(GameConfig::kFrameTime);
		}

		registry_.clear<Command>();
		++registry_.ctx<GameState>().run_frame;

		++run_frame_times;
	}
}

void Game::Finalize()
{
	for (auto& system : systems_)
	{
		system->Finalize();
	}

	registry_.clear();

	UnSetCtx();
}

void Game::InputCommand(uint32_t frame,uint32_t id, const Command& command)
{
	auto& group = GetCommandGroup(frame);
	group.commands.emplace(id,command);
}

uint32_t Game::GetVerifiedFrame() const
{
	return registry_.ctx<GameState>().verified_frame;
}

void Game::LoadScene(const std::string& name)
{
	const std::string path = "Config/Scene/";

	auto& locator = registry_.ctx<Locator>();
	auto content = locator.Ref<FileService>().OpenFile(path + name);

	assert(content != "" && "not scene info");

	nlohmann::json j = nlohmann::json::parse(content);
	auto scene = j.get<SceneInfo>();

	for (auto& obstacle : scene.obstacles())
	{
		auto e = registry_.create();
		registry_.emplace<Collider>(e, obstacle.collider);
		registry_.emplace<Transform>(e, obstacle.transform);
	}
}

void Game::CreatePlayer()
{
	for (auto& player_info : player_infos_)
	{
		actor_utiltiy::CreatePlayer(registry_, player_info.id, player_info.actor_asset, vec3(player_info.id * 2, 0, 0));
	}
}

CommandGroup& Game::GetCommandGroup(uint32_t frame)
{
	auto& command_group = command_groups_[frame % command_groups_.size()];
	// 清理旧的命令
	if (frame > command_group.frame)
	{
		command_group.commands.clear();
		command_group.frame = frame;
	}
	return command_group;
}
