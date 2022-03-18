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
		auto view = registry.view<Command>();
		for (auto e : view)
		{
			auto& command = view.get<Command>(e);
			
			BuildMoveCommand(command);
			BuildAbilityCommand(command);
		}
	}

	void BuildMoveCommand(const Command& command)
	{
		if (command.x_axis > 0 || command.y_axis > 0)
		{
			auto& move_command = registry.ctx_or_set<MoveCommand>();
			move_command.value.emplace_back(command.target, command.x_axis, command.y_axis);
		}
	}

	void BuildAbilityCommand(const Command& command)
	{
		for (auto& item : command.abilities)
		{
			if (item.second == AbilityStatus::kDown)
			{
				GameplayAbilityUtility::BuildGameplayAbilityCommand(registry, command.target, item.first);
			}
		}
	}

	void Finalize() override
	{

	}
};