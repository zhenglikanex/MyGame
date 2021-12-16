#pragma once

#include "Kanex.hpp"

struct GameplayAttributeData
{
	GameplayAttributeData()
		: base_value(0)
		, current_value(0)
	{

	}

	float base_value;
	float current_value;
};

NON_MEMEBER_BAR(GameplayAttributeData, obj.base_value,obj.current_value);