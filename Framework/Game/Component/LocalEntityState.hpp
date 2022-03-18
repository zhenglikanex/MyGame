#pragma once

#include "Framework/Game/GameConfig.hpp"

struct LocalEntityState
{
	std::array<uint32_t,GameConfig::kMaxPredictFrame> identifiers;
	std::array<uint32_t, GameConfig::kMaxPredictFrame> nums;
};