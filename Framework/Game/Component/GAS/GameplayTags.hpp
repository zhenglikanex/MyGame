#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayTags
{
	GameplayTagArray tags;
	std::unordered_map<entt::entity, GameplayTagArray&> attacth_tags;

	bool HasAny(const GameplayTagArray& array) const
	{
		return tags.HasAny(array);
	}

	bool HasAll(const GameplayTagArray& array) const
	{
		return tags.HasAll(array);
	}
};

void RegisterGameplayTags(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTags>("GameplayTags");
	type["tags"] = &GameplayTags::tags;
	type["attacth_tags"] = &GameplayTags::attacth_tags;
}
