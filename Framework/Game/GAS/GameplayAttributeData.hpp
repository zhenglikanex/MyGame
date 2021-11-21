#pragma once

#include "3rdparty/include/sol/sol.hpp"

struct GameplayAttributeData
{
	GameplayAttributeData()
		: base_value(0)
		, current_value(0)
	{

	}

	float base_value;
	float current_value;
};

void RegisterGameplayAttributeData(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayAttributeData>("GameplayAttributeData");
	type["base_value"] = &GameplayAttributeData::base_value;
	type["current_value"] = &GameplayAttributeData::current_value;
}