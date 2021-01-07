#pragma once

#include "Framework/Game/Math.hpp"

struct Transform
{
	Transform()
		:position(vec3(fixed16(0))),forward(vec3(fixed16(0)))
	{

	}
	Transform(vec3 _pos, vec3 _foward)
		:position(_pos),forward(_foward)
	{
	}

	vec3 position;
	vec3 forward;
	BAR(position,forward);
};