#pragma once

#include <string>
#include <string_view>

#include "Kanex.hpp"

#include "Framework/Game/Component/UniqueId.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayAbilitySpec
{
	std::string ability;
	uint32_t level;
	UniqueId source;
	UniqueId target;
	bool activation;

	GameplayAbilitySpec() = default;

	GameplayAbilitySpec(std::string_view _ability,uint32_t _level, UniqueId _source, UniqueId _target)
		: ability(_ability)
		, level(_level)
		, source(_source)
		, target(_target)
		, activation(false)
	{
	}
};

NON_MEMEBER_BAR(GameplayAbilitySpec, obj.ability, obj.level, obj.source, obj.target,obj.activation);