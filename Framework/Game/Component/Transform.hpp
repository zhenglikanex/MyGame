#pragma once

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

struct Transform
{
	vec3 position;
	quat rotation;

	Transform()
		: position(0)
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
		, rotation(glm::normalize(glm::quat_cast(m)))
	{

	}

	bool operator==(const Transform& other) const
	{
		return position == other.position && rotation == other.rotation;
	}

	bool operator!=(const Transform& other) const
	{
		return !(*this == other);
	}
};

inline mat4 GetMatrix4x4(const Transform& transform) 
{
	mat4 m = glm::mat4_cast(transform.rotation);
	m[3] = vec4(transform.position,1.0f);
	return m;
}

NON_MEMEBER_BAR(Transform, obj.position, obj.rotation);