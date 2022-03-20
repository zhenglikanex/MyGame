#pragma once

#include <vector>

#include "Framework/Game/Component/Remote.hpp"

struct MoveCommandSpec
{
	Remote target;

	float x;
	float y;

	MoveCommandSpec() = default;

	MoveCommandSpec(Remote _target, float _x, float _y)
		: target(_target)
		, x(_x)
		, y(_y)
	{

	}
};

NON_MEMEBER_BAR(MoveCommandSpec, obj.target,obj.x,obj.y);

struct MoveCommand
{
	std::vector<MoveCommandSpec> value;

};

NON_MEMEBER_BAR(MoveCommand, obj.value);