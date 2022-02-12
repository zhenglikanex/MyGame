#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct GameplayState
{
	std::array<GameplayAttributeSet, GameConfig::kMaxPredictFrame> attribute_sets;
	std::array<GameplayTagCountContainer, GameConfig::kMaxPredictFrame> tag_counts;
};