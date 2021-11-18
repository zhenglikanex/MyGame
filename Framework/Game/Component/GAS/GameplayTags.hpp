#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayTags
{
	std::unordered_map<GameplayEffect*, GameplayTagArray> value;

	bool HasAny(const GameplayTagArray& array) const
	{
		

		for (auto& item : value)
		{
			if (item.second.HasAny(array))
			{
				return true;
			}
		}

		return false;
	}

	bool HasAll(const GameplayTagArray& array) const
	{
		for (auto& item : value)
		{
			if (!item.second.HasAll(array))
			{
				return false;
			}
		}

		return true;
	}
};

void RegisterGameplayTags(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTags>("GameplayTags");
	type["value"] = &GameplayTags::value;
}
