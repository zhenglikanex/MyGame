#pragma once

#include <string>
#include <string_view>

#include "Kanex.hpp"

struct ActorAsset
{
	std::string value;

	ActorAsset() = default;

	ActorAsset(std::string_view _value)
		:value(_value)
	{

	}
};

NON_MEMEBER_BAR(ActorAsset, obj.value);