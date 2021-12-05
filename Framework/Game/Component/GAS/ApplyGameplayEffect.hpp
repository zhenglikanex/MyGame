#pragma once

#include <vector>
#include <string>

#include "3rdparty/include/sol/sol.hpp"
#include "3rdparty/include/entt/entt.hpp"

struct GameplayEffectInfo
{
	std::string name;
	uint32_t level;
	entt::entity source;
	entt::entity target;
};

struct ApplyGameplayEffect
{
	std::unordered_map<entt::entity, std::vector<GameplayEffectInfo>> value;
};
