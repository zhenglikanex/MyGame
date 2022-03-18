#pragma once

#include <vector>

#include "Framework/Game/Component/UniqueId.hpp"

struct MoveCommandSpec
{
	UniqueId target;

	float x;
	float y;

	MoveCommandSpec(UniqueId _target, float _x, float _y)
		: target(_target)
		, x(_x)
		, y(_y)
	{

	}
};

struct MoveCommand
{
	std::vector<MoveCommandSpec> value;
	
	MoveCommand(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) = delete;
};