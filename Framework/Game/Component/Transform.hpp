#pragma once

#include "Framework/Game/Math.hpp"

struct Transform
{
	vec3 position;
	quat rotation;

	Transform()
		: position(zero<vec3>())
		, rotation(quat(zero<vec3>()))
	{

	}

	Transform(const vec3& _pos, const quat& _rotation)
		: position(_pos)
		, rotation(_rotation)
	{
	}

	Transform(const mat4& m)
		: position(m[3])
		, rotation(glm::quat_cast(m))
	{

	}

	mat4 GetMatrix4x4() const 
	{
		mat4 m = glm::mat4_cast(rotation);
		m[3] = vec4(position,fixed16(1));
		return m;
	}
};
