#pragma once

#include "3rdparty/include/sol/sol.hpp"

struct GameplayAbilityScript
{
	sol::table value;

	GameplayAbilityScript(const sol::table& _value)
		: value(_value)
	{

	}
};