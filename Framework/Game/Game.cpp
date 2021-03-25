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

Game::Game(Locator&& locator,GameMode mode,std::vector<PlayerInfo>&& players)
	:player_infos_(players)
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
	systems_.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry_));
	systems_.emplace_back(std::make_unique<CollisionSystem>(registry_));
	systems_.emplace_back(std::make_unique<AnimationSystem>(registry_));
	systems_.emplace_back(std::make_unique<UpdateViewSystem>(registry_));

#ifdef DEBUG
	systems_.emplace_back(std::make_unique<DebugSystem>(registry_));
#endif
}

Game::~Game()
{

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

	
		
	if (registry_.ctx<GameMode>() == GameMode::kClinet)
	{
		UpdateClinet(dt);
	}
	else
	{
		UpdateServer(dt);
	}
}

void Game::UpdateClinet(float dt)
{
	uint32_t mill_dt = static_cast<uint32_t>(dt * 1000);

	auto& game_state = registry_.ctx<GameState>();
	game_state.run_time += mill_dt;

	// todo 检查预测

	// todo:后面根据延迟调整自己的输入频率
	while (game_state.run_time > game_state.run_frame * kFrameRate + kFrameRate)
	{
		auto start = std::chrono::system_clock::now();

		auto& input_service = registry_.ctx<Locator>().Ref<const InputService>();
		input_service.InputHandler();

		SetupCommands(game_state.run_frame);

		if (command_groups_.size() <= game_state.run_frame)
		{
			return;
		}

		auto command_group = command_groups_[game_state.run_frame];
		for (auto e : registry_.view<Player>())
		{
			const auto& player = registry_.get<Player>(e);
			auto iter = command_group.find(player.id);
			if (iter != command_group.end())
			{
				registry_.emplace_or_replace<Command>(e, iter->second);
			}
		}

		if (registry_.view<Command>().size() < registry_.view<Player>().size())
		{
			return;
		}

		for (auto& system : systems_)
		{
			system->Update(GameConfig::kFrameTime);
		}

		for (auto& system : systems_)
		{
			system->LateUpdate(GameConfig::kFrameTime);
		}

		registry_.clear<Command>();
		++registry_.ctx<GameState>().run_frame;

		auto t = std::chrono::duration<double>(std::chrono::system_clock::now() - start).count();
		//INFO("cpp time {}", t * 1000);
	}
}

void Game::UpdateServer(float dt)
{
	uint32_t mill_dt = static_cast<uint32_t>(dt * 1000);

	auto& game_state = registry_.ctx<GameState>();
	game_state.run_time += mill_dt;

	while (game_state.run_time > game_state.run_frame * kFrameRate + kFrameRate)
	{
		//生成command
		/*Setupcommands(game_state.run_frame);
		if (registry_.ctx<commandGroup>().value.size() == registry_.view<Player>().size())
		{
			for (auto& system : systems_)
			{
				system->Update(fixed16(0.33));
			}

			++registry_.ctx<GameState>().real_frame;
			++registry_.ctx<GameState>().run_frame;

			SaveSnapshot();
		}*/
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

void Game::InputCommand(uint32_t id,Command&& command)
{
	auto iter = player_input_commands_.find(id);
	if (iter != player_input_commands_.end())
	{
		iter->second.emplace_back(std::move(command));
	}
	else
	{
		std::vector<Command> commands;
		commands.emplace_back(command);
		player_input_commands_.emplace(id, std::move(commands));
	}
}

void Game::SetupCommands(uint32_t frame)
{
	CommandGroup group;
	for (auto& e : player_input_commands_)
	{
		if (e.second.size() <= frame)
		{
			if (e.second.empty())
			{
				group.emplace(e.first, Command());
			}
			else
			{
				group.emplace(e.first, e.second.back());
			}
		}
		else
		{
			group.emplace(e.first, e.second[frame]);
		}
	}

	command_groups_.push_back(std::move(group));
}

Command Game::PredictCommand(uint32_t id)
{
	auto iter = player_input_commands_.find(id);
	if (iter != player_input_commands_.end())
	{
		if (!iter->second.empty())
		{
			return iter->second.back();
		}
	}

	return Command();
}

void Game::CheckPredict()
{
	auto game_state = registry_.ctx<GameState>();
	for (uint32_t frame = game_state.real_frame; frame < game_state.run_frame; ++frame)
	{
		//auto command_group = GetCommandGroup(frame);
		//if (command_group.value.size() < registry_.view<Player>().size())
		//{
		//	return;
		//}

		//auto& predict_commands = predict_command_groups_.find(frame)->second;
		//if (command_group == predict_commands)
		//{
		//	predict_command_groups_.erase(game_state.real_frame);
		//	snapshots_.erase(game_state.real_frame);
		//	++registry_.ctx<GameState>().real_frame;
		//}
		//else
		//{
		//	auto pre_frame = frame - 1;
		//	auto& snapshot = snapshots_.find(pre_frame);
		//	//registry_.reset(snapshot);	// todo
		//	registry_.ctx<GameState>().real_frame = pre_frame;

		//	predict_command_groups_.clear();
		//	snapshots_.clear();
		//	return;
		//}
	}

	return;
}


void Game::SaveSnapshot()
{
	auto& game_state = registry_.ctx<GameState>();

	auto& snapshot = snapshots_.pop();
	snapshot.frame = game_state.run_frame;
	snapshot.buffer.Clear();

	kanex::BinaryStream stream(snapshot.buffer);
	kanex::BinaryOutputArchive ar(stream);
	entt::snapshot{ registry_ }
		.entities(ar)
		.component<
		ActorAsset,
		AnimationAsset,

		>(ar);

}

void Game::LoadSnapshot()
{

}

void Game::CreatePlayer()
{
	for (auto& player_info : player_infos_)
	{
		ActorUtiltiy::CreatePlayer(registry_, player_info.id, player_info.actor_asset);
		player_input_commands_.emplace(player_info.id, std::vector<Command>());
	}
}

void Game::LoadAllConfig()
{
	
}