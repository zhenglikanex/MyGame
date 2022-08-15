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

	GameplayAbilityCommandSpec() = default;

	GameplayAbilityCommandSpec(std::string_view _ability, Remote _source, Remote _target)
		: ability(_ability)
		, source(_source)
		, target(_target)
	{

	}
};

NON_MEMEBER_BAR(GameplayAbilityCommandSpec, obj.ability, obj.source, obj.target);

struct GameplayAbilityCommand
{
	std::unordered_map<Remote, std::vector<GameplayAbilityCommandSpec>> value;
};

NON_MEMEBER_BAR(GameplayAbilityCommand, obj.value);