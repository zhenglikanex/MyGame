#pragma once

#include <string>

#include "Kanex.hpp"

struct SkillGraphAsset
{
	std::string value;

	SkillGraphAsset() = default;
	SkillGraphAsset(std::string_view _value)
		: value(_value)
	{

	}
};

NON_MEMEBER_BAR(SkillGraphAsset, obj.value);