#pragma once

#include <array>

#include "Framework/Game/GAS/GameplayAttributeData.hpp"

#include "Kanex.hpp"

enum class GameplayAttributeType : uint8_t
{
	Hp,
	MaxHp,
	Mp,
	MaxMp,
	Defend,
	Attack,
	
	MaxCount
};

struct GameplayAttributeSet
{
	std::array<GameplayAttributeData, (size_t)GameplayAttributeType::MaxCount> value;

	GameplayAttributeSet() = default;	
};

NON_MEMEBER_BAR(GameplayAttributeSet, obj.value);