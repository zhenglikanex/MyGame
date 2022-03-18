#pragma once

#include <unordered_map>
#include <vector>

struct GameplayAbilityCommandSpec
{
	std::string ability;
	entt::entity source;
	entt::entity target;

	GameplayAbilityCommandSpec(std::string_view _ability, entt::entity _source, entt::entity _target)
		: ability(_ability)
		, source(_source)
		, target(_target)
	{

	}
};

struct GameplayAbilityCommand
{
	std::unordered_map<entt::entity, std::vector<GameplayAbilityCommandSpec>> value;
};