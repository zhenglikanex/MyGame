#pragma once

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/MoveCommand.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"

struct CommandSystem : public System
{
	CommandSystem(Registry& _registry)
		: System(_registry)
	{ }
	~CommandSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& game_state = registry.ctx<GameState>();
		auto& command_state = registry.ctx_or_set<CommandState>();

		auto& command = command_state.value[game_state.run_frame % command_state.value.size()];

		auto& move_command = registry.ctx_or_set<MoveCommand>();
		move_command.value = command.move_command.value;

		auto& ability_command = registry.ctx_or_set<GameplayAbilityCommand>();
		ability_command.value = command.gameplay_ability_command.value;
	}

	void Finalize() override
	{

	}
};