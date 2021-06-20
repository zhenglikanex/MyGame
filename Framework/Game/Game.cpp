#include "Game.hpp"

#include <filesystem>
#include <chrono>

#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/InputService.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/TransformState.hpp"

#include "Framework/Game/System.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"

#include "Framework/Game/Utility/ActorUtility.hpp"

using namespace std::chrono;

Game::Game(Locator&& locator,std::vector<PlayerInfo>&& players)
	: start_time_(0)
	, run_time_(0)
	, run_frame_(0)
	, real_frame_(0)
	, input_frame_rate_(GameConfig::kDefaultInputFrame)
	, player_infos_(players)
	
{
	registry_.set<Locator>(std::move(locator));
	registry_.set<GameState>();

	auto& game_helper = registry_.ctx<Locator>().Ref<const GameHelper&>();
	systems_ = game_helper.CreateSystems(registry_);

}

Game::~Game()
{
	registry_.clear();
}

bool Game::Initialize()
{
	for (auto& system : systems_)
	{
		if (!system->Initialize())
		{
			return false;
		}
	}

	auto& file_service = registry_.ctx<Locator>().Ref<FileService>();
	file_service.set_cur_path(std::filesystem::current_path().string() + "/Assets/Resources/");

	LoadAllConfig();
	CreatePlayer();

	return true;
}

void Game::Update(float dt)
{
	//run_time_ += dt;
	//INFO("START_TIME {}", start_time_);
	// todo:暂时用这种方式计算时间，dt精度不够，后面再改
	uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	run_time_ = (now - start_time_) / 1000.0f;

	UpdateInput();
	UpdateLogic();
}

void Game::UpdateInput()
{
	while (registry_.ctx<GameState>().run_frame == run_frame_ 
		&& (run_time_ > run_frame_ * input_frame_rate_ + input_frame_rate_) 
		&& (run_frame_ - real_frame_) < GameConfig::kMaxPredictFrame)
	{
		//会触发InputCommand,相当于getcommands
		auto& input_service = registry_.ctx<Locator>().Ref<const InputService>();
		input_service.InputHandler();

		auto& group = GetCommandGroup(run_frame_);

		assert((group.commands.size() == registry_.view<Player, Local>().size()) && "not commands");

		++run_frame_;
	}
}

void Game::UpdateLogic()
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
			if (dynamic_cast<CreateViewSystem*>(system.get()))
			{
				if (game_state.run_frame + 1 >= run_frame_)
				{
					system->FixedUpdate(GameConfig::kFrameTime);
				}
			}
			else 
			{
				system->FixedUpdate(GameConfig::kFrameTime);
			}
		}

		for (auto& system : systems_)
		{
			system->LateUpdate(GameConfig::kFrameTime);
		}

		for (auto& system : systems_)
		{
			auto observer_system = dynamic_cast<ObserverSystem*>(system.get());
			if (observer_system != nullptr)
			{
				observer_system->Clear();
			}
		}

		registry_.clear<Command>();
		++registry_.ctx<GameState>().run_frame;

		++run_frame_times;
	}
}

void Game::Finalize()
{
	for (auto iter = systems_.rbegin(); iter != systems_.rend(); ++iter)
	{
		auto& system = *iter;
		system->Finalize();
	}
}


void Game::InputCommand(uint32_t frame,uint32_t id, const Command& command)
{
	auto& group = GetCommandGroup(frame);
	group.commands.emplace(id,command);
}

void Game::Rollback(uint32_t frame)
{

}

void Game::UpdateFrameData(const uint32_t frame, const std::unordered_map<uint32_t, Transform>& frame_data)
{
	auto view = registry_.view<Player,Transform>();
	for (auto e : view)
	{
		const auto& player = registry_.get<Player>(e);
		auto& transform = registry_.get<Transform>(e);
		
		auto iter = frame_data.find(player.id);
		if (iter != frame_data.end()) // 没变动角色不会下发
		{
			bool is_local = registry_.has<Local>(e);
			if (is_local && frame < run_frame_)
			{
				// todo 可以允许有一定误差,因为只是预测，最终服务器都会会纠正
				bool is_need_rollback = transform.position != iter->second.position || transform.rotation != iter->second.rotation;
				if (is_need_rollback)
				{
					transform = iter->second;
				}
			}
			else
			{
				registry_.replace<Transform>(e,iter->second);
			}
		}
	}
}

uint32_t Game::GetCurRunFrame() const
{
	auto& game_state = registry_.ctx<GameState>();
	return game_state.run_frame;
}

void Game::CreatePlayer()
{
	for (auto& player_info : player_infos_)
	{ 
		ActorUtiltiy::CreatePlayer(registry_, player_info.id, player_info.actor_asset);
	}
}

void Game::LoadAllConfig()
{
	
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
