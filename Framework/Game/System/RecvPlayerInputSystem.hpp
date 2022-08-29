#pragma once

#include "Framework/Game/PlayerInput.hpp"
#include "Framework/Game/Service/NetworkService.hpp"

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
	}

	void InputGameState(uint32_t id,const GameState& game_state)
	{
		auto& game_state_map = registry.ctx_or_set<PlayerGameStateMap>();
		game_state_map.value[id] = game_state;
	}

	void InputCommand(Command& input_command)
	{
		auto& game_state = registry.ctx<GameState>();
		if (game_state.run_frame > input_command.frame)
		{
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
		command.gameplay_ability_command.value.insert(input_command.gameplay_ability_command.value.begin(), input_command.gameplay_ability_command.value.end());
	}

	void Finalize() override
	{

	}
};