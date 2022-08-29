#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/Component/MoveCommand.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityCommand.hpp"

struct Command
{
	uint32_t frame;
	MoveCommand move_command;
	GameplayAbilityCommand gameplay_ability_command;
};

NON_MEMEBER_BAR(Command, obj.frame,obj.move_command, obj.gameplay_ability_command);

struct CommandState
{
	std::array<Command, GameConfig::kMaxPredictFrame> value;
};

