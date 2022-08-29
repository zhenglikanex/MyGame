#pragma once

#include <unordered_map>

#include "Framework/Game/Component/GameState.hpp"

struct PlayerGameStateMap
{
	std::unordered_map<uint32_t, GameState> value;
};