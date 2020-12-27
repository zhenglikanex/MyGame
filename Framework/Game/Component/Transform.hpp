#pragma once

#include "Framework/Game/Math.hpp"

struct Transform
{
	Transform()
		:pos(vec3(fixed16(0))),forward(vec3(fixed16(0)))
	{

	}
	Transform(vec3 _pos, vec3 _foward)
		:pos(_pos),forward(_foward)
	{
	}

	vec3 pos;
	vec3 forward;
	BAR(pos,forward);
};