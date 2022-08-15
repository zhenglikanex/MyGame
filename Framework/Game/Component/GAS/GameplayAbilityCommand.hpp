#pragma once

#include <unordered_map>
#include <vector>

#include "Framework/Game/Component/Remote.hpp"

#include "Kanex.hpp"

struct GameplayAbilityCommandSpec
{
	std::string ability;
	Remote source;
	Remote target;
	bool activated;

	GameplayAbilityCommandSpec() = default;

	GameplayAbilityCommandSpec(std::string_view _ability, Remote _source, Remote _target, bool _activated)
		: ability(_ability)
		, source(_source)
		, target(_target)
		, activated(_activated)
	{

	}
};

NON_MEMEBER_BAR(GameplayAbilityCommandSpec, obj.ability, obj.source, obj.target, obj.activated);

struct GameplayAbilityCommand
{
	std::unordered_map<Remote,std::unordered_map<std::string,GameplayAbilityCommandSpec>> value;
};

NON_MEMEBER_BAR(GameplayAbilityCommand, obj.value);