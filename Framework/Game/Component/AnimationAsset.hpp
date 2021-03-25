#pragma once

#include <string>
#include <string_view>

struct AnimationAsset
{
	std::string value;

	AnimationAsset() = default;
	AnimationAsset(std::string_view _value)
		: value(_value)
	{

	}
};

NON_MEMEBER_BAR(AnimationAsset, obj.value);