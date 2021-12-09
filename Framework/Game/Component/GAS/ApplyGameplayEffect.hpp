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

	GameplayEffectInfo(std::string_view _name, uint32_t _level, entt::entity _source, entt::entity _target)
		: name(_name)
		, level(_level)
		, source(_source)
		, target(_target)
	{

	}
};

struct ApplyGameplayEffect
{
	std::unordered_map<entt::entity, std::vector<GameplayEffectInfo>> value;
};
