#pragma once

#include <vector>
#include <string>

#include "3rdparty/include/sol/sol.hpp"

struct GameplayEffectInfo
{
	std::string name;
	uint32_t level;
	entt::entity target;
};

struct ApplyGameplayEffectList
{
	std::vector<GameplayEffectInfo> value;
};

void RegisterApplyGameplayEffectList(sol::state& lua)
{
	sol::usertype<ApplyGameplayEffectList> type = lua.new_usertype<ApplyGameplayEffectList>("ApplyGameplayEffectList");
	type["value"] = &ApplyGameplayEffectList::value;
}