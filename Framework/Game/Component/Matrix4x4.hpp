#pragma once

#include "Framework/Game/Math.hpp"

struct Matrix4x4
{
	mat4 value;

	Matrix4x4()
		:value(glm::identity<mat4>())
	{

	}

	Matrix4x4(const mat4& _value)
		:value(_value)
	{

	}
};