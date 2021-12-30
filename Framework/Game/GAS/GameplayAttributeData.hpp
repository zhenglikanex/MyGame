#pragma once

#include <numeric>

#include "Kanex.hpp"

struct GameplayAttributeData
{
	float base_value;
	float current_value;

	GameplayAttributeData()
		: base_value(0)
		, current_value(0)
	{

	}

	bool operator==(const GameplayAttributeData& other) const
	{
		return std::fabs(base_value - other.base_value) <= std::numeric_limits<float>::epsilon() &&
			std::fabs(current_value - other.current_value) <= std::numeric_limits<float>::epsilon();
	}

	bool operator!=(const GameplayAttributeData& other) const
	{
		return !(*this == other);
	}

};

NON_MEMEBER_BAR(GameplayAttributeData, obj.base_value,obj.current_value);