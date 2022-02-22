#pragma once

#include <string>
#include <vector>

#include "Kanex.hpp"

struct GameplayInputSpec
{
	std::string key;
	std::string pressed;
	std::string released;

	GameplayInputSpec() = default;

	GameplayInputSpec(std::string_view _key,std::string_view _pressed,std::string_view _released)
		: key(_key)
		, pressed(_pressed)
		, released(_released)
	{

	}
};

NON_MEMEBER_BAR(GameplayInputSpec,obj.key,obj.pressed,obj.released)

struct GameplayInputTask
{
	std::unordered_map<std::string, GameplayInputSpec> value;
};

NON_MEMEBER_BAR(GameplayInputTask, obj.value);