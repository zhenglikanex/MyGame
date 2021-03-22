#pragma once

#include <string>

#include "Framework/Game/Data/AnimationConfig.hpp"

struct Animation
{
	const AnimationInfo* value;

	Animation(const AnimationInfo* _value)
		:value(_value)
	{
	}
};

