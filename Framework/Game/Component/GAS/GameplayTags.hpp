#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayTags
{
	GameplayTagArray tags;
	std::unordered_map<entt::entity, GameplayTagArray> attacth_tags;

	bool HasAny(const GameplayTagArray& array) const
	{
		return tags.HasAny(array);
	}

	bool HasAll(const GameplayTagArray& array) const
	{
		return tags.HasAll(array);
	}
};
