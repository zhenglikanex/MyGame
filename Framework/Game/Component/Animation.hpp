#pragma once

#include <string>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/Game/Data/AnimationConfig.hpp"

#include "Framework/serialize/binary_archive.hpp"


struct Animation
{
	const AnimationInfo* value;

	Animation(const AnimationInfo* _value)
		:value(_value)
	{
	}
};

