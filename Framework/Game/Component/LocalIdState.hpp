#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/LocalId.hpp"

struct LocalIdState
{
	std::array<LocalId, GameConfig::kMaxPredictFrame> value;
};