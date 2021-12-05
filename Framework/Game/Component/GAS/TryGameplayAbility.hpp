#pragma once

#include <unordered_map>
#include <vector>

#include "Framework/Game/GAS/GameplayAbility.hpp"

struct GameplayAbilityInfo
{
	std::string name;
	uint32_t level;
	entt::entity source;
	entt::entity target;
};

struct TryGameplayAbility
{
	std::unordered_map<entt::entity, std::vector<GameplayAbilityInfo>> value;
};