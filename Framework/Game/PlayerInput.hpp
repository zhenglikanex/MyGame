#pragma once

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/GameState.hpp"

struct PlayerInput
{
	uint32_t id{};
	GameState game_state{};
	Command command{};
};

NON_MEMEBER_BAR(PlayerInput, obj.id, obj.game_state,obj.command);