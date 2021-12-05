#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayAbilitySpec
{
	GameplayAbility& ga;
	uint32_t level;
	entt::entity source;
	entt::entity target;

	GameplayAbilitySpec(GameplayAbility& _ga, uint32_t _level, entt::entity _source, entt::entity _target)
		: ga(_ga)
		, level(_level)
		, source(_source)
		, target(_target)
	{

	}
};

