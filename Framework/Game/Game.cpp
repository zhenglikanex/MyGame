#include "Game.hpp"

#include <filesystem>

#include "Framework/Game/InputService.hpp"
#include "Framework/Game/LogService.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Asset.hpp"
#include "Framework/Game/Component/MainPlayer.hpp"

#include "Framework/Game/System/CommandProcessSystem.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/UpdateViewSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/CollisionSystem.hpp"
#include "Framework/Game/System/SyncSystem.hpp"

#include "Framework/Game/Data/RootMotionConfig.hpp"

Game::Game(Locator&& locator,GameMode mode,std::vector<Player>&& players)
{
	registry_.set<GameMode>(mode);
	registry_.set<Locator>(std::move(locator));
	registry_.set<GameState>();
	registry_.set<commandGroup>();
	
	systems_.emplace_back(std::make_unique<CommandProcessSystem>(registry_));
	systems_.emplace_back(std::make_unique<CreateViewSystem>(registry_));
	systems_.emplace_back(std::make_unique<MovementSystem>(registry_));
	systems_.emplace_back(std::make_unique<AnimationSystem>(registry_));
	systems_.emplace_back(std::make_unique<UpdateViewSystem>(registry_));
	systems_.emplace_back(std::make_unique<CollisionSystem>(registry_));
	systems_.emplace_back(std::make_unique<SyncSystem>(registry_));

	for (auto player : players)
	{
		auto e = registry_.create();
		registry_.emplace<Player>(e, player);
		
		//command command;
		//Inputcommand(player.id,std::move(command));

		main_player_id_ = player.id;
		registry_.set<MainPlayer>(main_player_id_);
	}
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
	//log_service.Info("current_path:{}", file_service.cur_path());
	LoadConfig<RootMotionConfig>("Config/Anim/HeroRootMotion.json");

	auto root_motions = registry_.ctx<RootMotionConfig>().GetEntry("Locomotion");
	for (auto e : registry_.view<Player>())
	{
		registry_.emplace<Asset>(e, "Hero");
	}

	SaveSnapshot();

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
	uint32_t mill_dt = dt * 1000;

	auto& game_state = registry_.ctx<GameState>();
	game_state.run_time += mill_dt;

	while (game_state.run_time > game_state.run_frame * kFrameRate + kFrameRate)
	{
		//todo:临时填充帧
		//Inputcommand(main_player_id_, command());
		// 进行
		// todo:后面根据延迟调整自己的输入频率
		auto& input_service = registry_.ctx<Locator>().Ref<const InputService>();
		input_service.InputHandler();

		bool predict = false;
		auto command_group = GetcommandGroup(game_state.run_frame);
		if (command_group.value.size() < registry_.view<Player>().size() && game_state.run_frame - game_state.real_frame <= kMaxPredictFrame)
		{
			predict = true;
			command_group = PredictcommandGroup(game_state.run_frame);
			SaveSnapshot();
		}

		if (command_group.value.size() == registry_.view<Player>().size())
		{
			registry_.set<commandGroup>(std::move(command_group));
			for (auto& system : systems_)
			{
				system->Update(fixed16(0.33));
			}

			if (predict)
			{
				++registry_.ctx<GameState>().run_frame;
			}
			else
			{
				++registry_.ctx<GameState>().real_frame;
				++registry_.ctx<GameState>().run_frame;
			}
		}
		else 
		{
			// 如果没有当前帧的命令，就跳出当前逻辑帧
			break;
		}
	}
}

void Game::UpdateServer(float dt)
{
	uint32_t mill_dt = dt * 1000;

	auto& game_state = registry_.ctx<GameState>();
	game_state.run_time += mill_dt;

	while (game_state.run_time > game_state.run_frame * kFrameRate + kFrameRate)
	{
		//生成command
		Setupcommands(game_state.run_frame);
		registry_.set<commandGroup>(GetcommandGroup(game_state.run_frame));
		if (registry_.ctx<commandGroup>().value.size() == registry_.view<Player>().size())
		{
			for (auto& system : systems_)
			{
				system->Update(fixed16(0.33));
			}

			++registry_.ctx<GameState>().real_frame;
			++registry_.ctx<GameState>().run_frame;

			SaveSnapshot();
		}
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

void Game::Inputcommand(uint32_t id,Command&& command)
{
	auto iter = commands_map_.find(id);
	if(iter != commands_map_.end())
	{
		iter->second.emplace_back(std::move(command));
	}
	else
	{
		std::vector<Command> commands;
		commands.emplace_back(std::move(command));
		commands_map_.emplace(id, std::move(commands));
	}

	if (registry_.ctx<GameMode>() == GameMode::kClinet)
	{
		//CheckPredict();
	}
}

void Game::Setupcommands(uint32_t frame)
{
	if (registry_.ctx<GameMode>() == GameMode::kServer)
	{
		for (auto& it : commands_map_)
		{
			if (it.second.size() == 0)
			{
				it.second.emplace_back(Command());
			}
			else if (it.second.size() <= frame)
			{
				it.second.push_back(it.second.back());
			}
		}
	}
}

commandGroup Game::GetcommandGroup(uint32_t frame)
{
	commandGroup commands;
	for(auto& it : commands_map_)
	{
		if (it.second.size() > frame) {
			commands.value.emplace(it.first, it.second[frame]);
		}
	}

	return commands;
}

std::vector<commandGroup> Game::GetAllcommandGroups()
{
	std::vector<commandGroup> command_groups;
	for (uint32_t frame = 0;frame <= registry_.ctx<GameState>().real_frame; ++frame)
	{
		command_groups.emplace_back(GetcommandGroup(frame));
	}
	return command_groups;
}

void Game::CheckPredict()
{
	auto game_state = registry_.ctx<GameState>();
	for (uint32_t frame = game_state.real_frame; frame < game_state.run_frame; ++frame)
	{
		auto command_group = GetcommandGroup(frame);
		if (command_group.value.size() < registry_.view<Player>().size())
		{
			return;
		}

		auto& predict_commands = predict_command_groups_.find(frame)->second;
		if (command_group == predict_commands)
		{
			predict_command_groups_.erase(game_state.real_frame);
			snapshots_.erase(game_state.real_frame);
			++registry_.ctx<GameState>().real_frame;
		}
		else
		{
			auto pre_frame = frame - 1;
			auto& snapshot = snapshots_.find(pre_frame);
			//registry_.reset(snapshot);	// todo�ָ�����
			registry_.ctx<GameState>().real_frame = pre_frame;

			// ���Ԥ�������
			predict_command_groups_.clear();
			snapshots_.clear();
			return;
		}
	}

	return;
}


commandGroup Game::PredictcommandGroup(uint32_t frame)
{
	commandGroup commands;
	for (auto& it : commands_map_)
	{
		if (it.second.size() > frame) {
			commands.value.emplace(it.first, it.second[frame]);
		}
		else
		{
			commands.value.emplace(it.first, it.second.back());
		}
	}

	return commands;
}

void Game::SaveSnapshot()
{
	Snapshot snapshot;
	snapshots_.emplace(registry_.ctx<GameState>().run_frame,std::move(snapshot));
}