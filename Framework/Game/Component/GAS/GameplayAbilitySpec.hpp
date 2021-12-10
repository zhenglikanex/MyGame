#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayAbilitySpec
{
	const GameplayAbility* ga;
	sol::table script;
	uint32_t level;
	entt::entity source;
	entt::entity target;

	GameplayAbilitySpec(const GameplayAbility* _ga,sol::table& _script,uint32_t _level, entt::entity _source, entt::entity _target)
		: ga(_ga)
		, script(_script)
		, level(_level)
		, source(_source)
		, target(_target)
	{

	}
};

