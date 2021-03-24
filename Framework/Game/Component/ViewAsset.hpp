#pragma once

#include <string>
#include <string_view>

struct ViewAsset
{
	std::string value;

	ViewAsset() = default;
	ViewAsset(std::string_view _value)
		: value(_value)
	{

	}
};

NON_MEMEBER_BAR(ViewAsset, obj.value);