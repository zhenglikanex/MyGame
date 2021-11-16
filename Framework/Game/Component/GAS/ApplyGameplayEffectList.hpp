#pragma once

#include <vector>
#include <string>

#include "3rdparty/include/sol/sol.hpp"

struct ApplyGameplayEffectList
{
	std::vector<std::string> value;
};

void RegisterApplyGameplayEffectList(sol::state& lua)
{
	sol::usertype<ApplyGameplayEffectList> type = lua.new_usertype<ApplyGameplayEffectList>("ApplyGameplayEffectList");
	type["value"] = &ApplyGameplayEffectList::value;
}