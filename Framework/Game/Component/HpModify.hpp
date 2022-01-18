#pragma once

#include <vector>

#include "Framework/Game/Component/UniqueId.hpp"

#include "Kanex.hpp"

struct HpModifySpec
{
	UniqueId source;
	UniqueId target;

	float value;

	HpModifySpec() = default;

	HpModifySpec(UniqueId _source, UniqueId _target, float _value)
		: source(_source)
		, target(_target)
		, value(_value)
	{

	}
};

NON_MEMEBER_BAR(HpModifySpec, obj.source, obj.target, obj.value);

struct HpModify
{
	std::vector<HpModifySpec> value;
};

NON_MEMEBER_BAR(HpModify, obj.value);