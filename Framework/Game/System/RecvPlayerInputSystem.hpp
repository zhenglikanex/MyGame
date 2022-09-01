#pragma once

#include "Framework/Game/PlayerInput.hpp"
#include "Framework/Game/Service/NetworkService.hpp"
#include "Framework/Game/Component/Remote.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/PlayerGameStateMap.hpp"

#include "Framework/Game/System.hpp"

struct RecvPlayerInputSystem : public System
{
	explicit RecvPlayerInputSystem(Registry& _registry)
		: System(_registry)
	{ }

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		//INFO("--------------------------------------------------------");
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		while (auto result = network_service.Recv("player_input"))
		{
			auto& data = *result;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			PlayerInput input;
			ar(input);

			InputGameState(input.id, input.game_state);
			InputCommand(input.command);
		}

		TestCommand();
	}

	void InputGameState(uint32_t id,const GameState& game_state)
	{
		auto& game_state_map = registry.ctx_or_set<PlayerGameStateMap>();
		game_state_map.value[id] = game_state;

		auto& server_game_state = registry.ctx<GameState>();
		//INFO("server run frame {} local run frame {}", server_game_state.run_frame,game_state.run_frame);
		//INFO("server real frame {} local real frame {}", server_game_state.real_frame, game_state.real_frame);
	}

	void InputCommand(Command& input_command)
	{
		auto& game_state = registry.ctx<GameState>();

		INFO("input_command {} {}", game_state.run_frame, input_command.frame);

		if (game_state.run_frame > input_command.frame)
		{
			INFO("command miss server run frame {} local run frame {}", game_state.run_frame, input_command.frame);
			return;
		}

		auto& command_state = registry.ctx_or_set<CommandState>();
		auto& command = command_state.value[input_command.frame % command_state.value.size()];

		if (command.frame != input_command.frame)
		{
			command.frame = input_command.frame;
			command.move_command.value.clear();
			command.gameplay_ability_command.value.clear();
		}

		command.move_command.value.insert(command.move_command.value.end(), input_command.move_command.value.begin(), input_command.move_command.value.end());
		command.gameplay_ability_command.value.insert(command.gameplay_ability_command.value.end(), input_command.gameplay_ability_command.value.begin(),input_command.gameplay_ability_command.value.end());
	}

	void TestCommand()
	{
		auto& game_state = registry.ctx_or_set<GameState>();
		auto& command_state = registry.ctx_or_set<CommandState>();
		auto& command = command_state.value[game_state.run_frame % command_state.value.size()];

		if (command.frame != game_state.run_frame)
		{
			command.frame = game_state.run_frame;
			command.move_command.value.clear();
			command.gameplay_ability_command.value.clear();
		}

		auto view = registry.view<Player,Remote>();
		for(auto e : view)
		{
			auto source = view.get<Remote>(e);

			command.gameplay_ability_command.value.emplace_back("GA_JumpAttackA", source, Remote::null, true);
			command.gameplay_ability_command.value.emplace_back("GA_LightAttackB", source, Remote::null, true);
			command.gameplay_ability_command.value.emplace_back( "GA_LightAttackC", source ,Remote::null,true);
			command.gameplay_ability_command.value.emplace_back("GA_LightAttackA", source, Remote::null, true);
		}
	}

	void Finalize() override
	{

	}
};