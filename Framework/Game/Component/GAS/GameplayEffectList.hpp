#pragma once

#include <vector>
#include <cstdint>

#include "3rdparty/include/sol/sol.hpp"

struct GameplayEffectList
{
	//GameplayEffectÊÇ¸ölua table
	std::vector<sol::table> value;
};

void RegisterGameplayEffectList(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayEffectList>("GameplayEffectList");
	type["value"] = &GameplayEffectList::value;
}