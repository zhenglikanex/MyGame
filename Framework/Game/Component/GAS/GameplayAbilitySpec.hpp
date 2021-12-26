#pragma once

#include <string>
#include <string_view>

#include "Kanex.hpp"

#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "Framework/Game/Component/UniqueId.hpp"

struct GameplayAbilitySpec
{
	std::string ability;
	sol::table script;
	uint32_t level;
	UniqueId source;
	UniqueId target;

	GameplayAbilitySpec() = default;

	GameplayAbilitySpec(std::string_view _ability,const sol::table& _script,uint32_t _level, UniqueId _source, UniqueId _target)
		: ability(_ability)
		, script(_script)
		, level(_level)
		, source(_source)
		, target(_target)
	{
	}
};

NON_MEMEBER_BAR(GameplayAbilitySpec, obj.ability, obj.level, obj.source, obj.target);