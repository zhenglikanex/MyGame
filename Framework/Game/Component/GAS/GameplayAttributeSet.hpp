#pragma once

#include <array>

#include "Framework/Game/GAS/GameplayAttributeData.hpp"

enum class GameplayAttributeType : uint8_t
{
	Hp,
	MaxHp,
	Mp,
	MaxMp,

	MaxCount
};

struct GameplayAttributeSet
{
	std::array<GameplayAttributeData, (size_t)GameplayAttributeType::MaxCount> value;
};

void RegisterGameplayAttributeSet(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayAttributeSet>("GameplayAttributeSet");
	type["value"] = &GameplayAttributeSet::value;
	
}