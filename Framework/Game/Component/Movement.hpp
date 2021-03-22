#pragma once

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

struct Movement
{
	vec3 forward;
	vec3 velocity;
	vec3 angular_velocity;

	Movement() = default;

	Movement(const vec3& _forward, const vec3& _velocity, const vec3& _angular_velocity)
		: forward(_forward)
		, velocity(_velocity)
		, angular_velocity(_angular_velocity)
	{

	}
};

NON_MEMEBER_BAR(Movement, obj.forward, obj.velocity, obj.angular_velocity);