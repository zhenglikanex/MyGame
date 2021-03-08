#pragma once

#include "Framework/Game/Math.hpp"

struct Transform
{
	Transform()
		: position(zero<vec3>())
		, mat(zero<mat4>())
		, rotation(quat(zero<vec3>()))
	{

	}
	Transform(const vec3& _pos, const quat& _rotation)
		:position(_pos)
		,mat(zero<mat4>())
		, rotation(_rotation)
	{
	}

	vec3 position;
	mat4 mat;
	quat rotation;
};