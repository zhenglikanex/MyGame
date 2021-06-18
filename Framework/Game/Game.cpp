#include "Game.hpp"

#include <filesystem>
#include <chrono>

#include "Framework/Game/InputService.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"

#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreateSkillGraphSystem.hpp"
#include "Framework/Game/System/ActorStateSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/UpdateViewSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/CollisionSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/SkillStateSystem.hpp"
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/HealthSystem.hpp"
#include "Framework/Game/System/ModifyHealthSystem.hpp"
#include "Framework/Game/System/SkillSystem.hpp"
#include "Framework/Game/System/UpdateColliderTransformSystem.hpp"

#include "Framework/Game/Utility/ActorUtility.hpp"

using namespace std::chrono;

Game::Game(Locator&& locator,GameMode mode,std::vector<PlayerInfo>&& players)
	: start_time_(0)
	, run_time_(0)
	, run_frame_(0)
	, real_frame_(0)
	, input_frame_rate_(GameConfig::kDefaultInputFrame)
	, player_infos_(players)
	
{
	registry_.set<GameMode>(mode);
	registry_.set<Locator>(std::move(locator));
	registry_.set<GameState>();

	systems_.emplace_back(std::make_unique<CreateActorSystem>(registry_));
	systems_.emplace_back(std::make_unique<CreateViewSystem>(registry_));
	systems_.emplace_back(std::make_unique<CreateAnimationSystem>(registry_));
	systems_.emplace_back(std::make_unique<CreateSkillGraphSystem>(registry_));
	systems_.emplace_back(std::make_unique<HealthSystem>(registry_));
	systems_.emplace_back(std::make_unique<ModifyHealthSystem>(registry_));
	systems_.emplace_back(std::make_unique<ActorStateSystem>(registry_));
	systems_.emplace_back(std::make_unique<SkillStateSystem>(registry_));
	systems_.emplace_back(std::make_unique<RootMotionSystem>(registry_));
	systems_.emplace_back(std::make_unique<MovementSystem>(registry_));
	systems_.emplace_back(std::make_unique<SkillSystem>(registry_));
	systems_.emplace_back(std::make_unique<AnimationSystem>(registry_));
	systems_.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry_));
	systems_.emplace_back(std::make_unique<CollisionSystem>(registry_));
	systems_.emplace_back(std::make_unique<UpdateViewSystem>(registry_));

#ifdef DEBUG
	systems_.emplace_back(std::make_unique<DebugSystem>(registry_));
#endif
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

		//预测玩家操作
		if (group.commands.size() < player_infos_.size())
		{
			if (run_frame_ == 0)
			{
				for (auto& player_info : player_infos_)
				{
					if (group.commands.find(player_info.id) == group.commands.end())
					{
						group.commands.emplace(player_info.id, Command());
					}
				}
			}
			else
			{
				// 简单的用上一帧命令填充(后续预测优化)
				auto& last_group = GetCommandGroup((run_frame_ - 1));
				for (auto& last_command : last_group.commands)
				{
					if (group.commands.find(last_command.first) == group.commands.end())
					{
						group.commands.emplace(last_command.first, last_command.second);
					}
				}
			}

			++run_frame_;
		}
		else
		{
			++real_frame_;
			++run_frame_;
		}
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

		// 如果是预测执行,保存快照
		if (game_state.run_frame >= real_frame_)
		{
			SaveSnapshot();
		}

		for (auto e : registry_.view<Player>())
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
					system->Update(GameConfig::kFrameTime);
				}
			}
			else 
			{
				system->Update(GameConfig::kFrameTime);
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

void Game::SaveSnapshot()
{
	auto& game_state = registry_.ctx<GameState>();

	auto& snapshot = snapshots_[game_state.run_frame % snapshots_.size()];
	snapshot.frame = game_state.run_frame;
	snapshot.buffer.Clear();

	kanex::BinaryStream stream(snapshot.buffer);
	kanex::BinaryOutputArchive ar(stream);

	entt::snapshot{ registry_ }
		.entities(ar)
		.component<
		ActorAsset,
		ActorState,
		EnterActorState,
		AnimationAsset,
		AnimationClip,
		AttributeUnitList,
		Collider,
		ColliderInfo,
		Command,
		ContactList,
		Health,
		ModifyHealthList,
		Matrix4x4,
		Movement,
		Player,
		Skill,
		SkillAttacthBone,
		SkillGraphAsset,
		SkillState,
		EnterSkillState,
		ExitSkillState,
		SkillParams,
		Transform,
		ViewAsset,
		Weapon
		>(ar);

	ar(registry_.ctx<GameState>());
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
