#pragma once

#include <string>
#include <string_view>

struct ActorAsset
{
	std::string value;

	ActorAsset() = default;

	ActorAsset(std::string_view _value)
		:value(_value)
	{

	}
};