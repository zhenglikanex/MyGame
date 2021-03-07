#pragma once

#include "Framework/Game/Math.hpp"

struct Transform
{
	Transform()
		:position(zero<vec3>()),forward(zero<vec3>()), mat(zero<mat4>())
	{

	}
	Transform(const vec3& _pos, const vec3& _foward)
		:position(_pos),forward(_foward),mat(zero<mat4>())
	{
	}

	vec3 position;
	vec3 forward;
	mat4 mat;
	BAR(position,forward);
};