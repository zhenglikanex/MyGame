#pragma once

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

struct Transform
{
	vec3 position;
	quat rotation;

	Transform()
		: position(zero<vec3>())
		, rotation(glm::identity<quat>())
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
};

inline mat4 GetMatrix4x4(const Transform& transform) 
{
	mat4 m = glm::mat4_cast(transform.rotation);
	m[3] = vec4(transform.position, fixed16(1));
	return m;
}

NON_MEMEBER_BAR(Transform, obj.position, obj.rotation);