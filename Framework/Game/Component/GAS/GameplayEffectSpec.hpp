#pragma once

#include <vector>
#include <cstdint>

#include "Framework/Game/GAS/GameplayEffect.hpp"

struct GameplayEffectSpec
{
	GameplayEffect& ge;
	entt::entity source;
	entt::entity owner;
	uint8_t level;
	float period;
	float duration;
	uint32_t stack_count;

	GameplayEffectSpec(GameplayEffect& _ge,entt::entity _source,entt::entity _owner,uint8_t _level)
		: ge(_ge)
		, source(_source)
		, owner(_owner)
		, level(_level)
		, period(0)
		, duration(0)
		, stack_count(1)
	{

	}
};