#pragma once

#include <unordered_map>
#include <vector>

#include "Framework/Game/GAS/GameplayAbility.hpp"

struct GameplayAbilityInfo
{
	std::string ability;
	entt::entity source;
	entt::entity target;

	GameplayAbilityInfo(std::string_view _ability, entt::entity _source, entt::entity _target)
		: ability(_ability)
		, source(_source)
		, target(_target)
	{

	}
};

struct TryGameplayAbility
{
	std::unordered_map<entt::entity, std::vector<GameplayAbilityInfo>> value;
};