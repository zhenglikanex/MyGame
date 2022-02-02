#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

#include "Framework/Game/Component/UniqueId.hpp"

#include "Kanex.hpp"

struct GameplayEffectSpec
{
	std::string effect;
	UniqueId source;
	UniqueId target;
	uint8_t level;
	float duration;
	float time;
	float period;
	uint32_t stack_count;
	std::unordered_map<GameplayAttributeType, float> source_snapshot_attrs;
	std::unordered_map<GameplayAttributeType, float> target_snapshot_attrs;
	GameplayTagArray dynamic_granted_tags;

	GameplayEffectSpec() = default;

	GameplayEffectSpec(std::string_view _effect,UniqueId _source,UniqueId _target,uint8_t _level,float _duration)
		: effect(_effect)
		, source(_source)
		, target(_target)
		, level(_level)
		, duration(_duration)
		, time(0)
		, period(0)
		, stack_count(1)
	{

	}
};

NON_MEMEBER_BAR(GameplayEffectSpec, obj.effect, obj.source, obj.target, obj.level, obj.period, obj.duration, obj.stack_count);