#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

struct GameplayState
{
	std::array<GameplayAttributeSet, GameConfig::kMaxPredictFrame> attribute_sets;
	std::array<GameplayTags, GameConfig::kMaxPredictFrame> tags;
};