#pragma once

#include <string>
#include <vector>

#include "Kanex.hpp"

struct GameplayInputSpec
{
	std::string key;
	std::string pressed;
	std::string released;
	bool is_pressed;

	GameplayInputSpec() = default;

	GameplayInputSpec(std::string_view _key,std::string_view _pressed,std::string_view _released)
		: key(_key)
		, pressed(_pressed)
		, released(_released)
		, is_pressed(false)
	{

	}
};

NON_MEMEBER_BAR(GameplayInputSpec,obj.key,obj.pressed)

struct GameplayInputTask
{
	std::unordered_map<std::string, GameplayInputSpec> value;
};

NON_MEMEBER_BAR(GameplayInputTask, obj.value);