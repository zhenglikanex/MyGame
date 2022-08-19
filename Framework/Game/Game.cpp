#include "Game.hpp"

#include <chrono>

#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/LuaBridge.hpp"
#include "Framework/Game/Registry.hpp"

#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/GameState.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorUtility.hpp"

#include "Framework/Game/Data/SceneConfig.hpp"

Game::Game(std::unique_ptr<GameHelper> game_helper, Locator locator,std::vector<PlayerInfo> players)
	: cur_frame_time_(0)
	, game_helper_(std::move(game_helper))
	, registry_(game_helper_->GetGameMode(), std::move(locator))
	, player_infos_(players)
{

}

Game::~Game()
{
	
}

bool Game::Initialize()
{
	SetCtx();

	InitializeSyncInstances();

	RegisterLua(registry_, registry_.ctx<sol::state>());

	LoadScene("Battle.json");

	CreatePlayer();

	game_helper_->CreateSystems(this);

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
	auto& sync_manager = registry_.set<SyncManager>();
	for (auto& info : player_infos_)
	{
		sync_manager.sync_instances.emplace(info.id, SyncInstance(info.id, 0));
	}

	if (game_helper_->GetGameMode() == GameMode::kAuthority)
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

void Game::Update(uint64_t dt)
{
	auto& game_state = registry_.ctx<GameState>();

	cur_frame_time_ += dt;
	uint32_t frame_time = game_helper_->GameFrameTime();
	while(cur_frame_time_ >= frame_time)
	{
		cur_frame_time_ -= frame_time;
		++game_state.real_frame;
	}

	UpdateLogic(dt / 1000000000.f);
	UpdateFixedLogic();
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

	INFO("begin UpdateFixedLogic {}", game_state.real_frame - game_state.run_frame);
	while (game_state.run_frame < game_state.real_frame)
	{
		INFO("while UpdateFixedLogic");
		for (auto& system : systems_)
		{
			system->FixedUpdate(GameConfig::kFrameTime);
		}

		for (auto& system : systems_)
		{
			system->LateUpdate(GameConfig::kFrameTime);
		}

		++game_state.run_frame;
		++run_frame_times;
	}

	INFO("end UpdateFixedLogic");
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