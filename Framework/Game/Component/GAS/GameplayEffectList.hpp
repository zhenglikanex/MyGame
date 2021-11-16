#pragma once

#include <vector>
#include <cstdint>

#include "3rdparty/include/sol/sol.hpp"

struct GameplayEffectList
{
	//GameplayEffectÊÇ¸ölua table
	std::vector<sol::table> value;
};
