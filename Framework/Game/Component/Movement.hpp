#pragma once

#include "Framework/Game/Math.hpp"
#include "Framework/serialize/binary_archive.hpp"

struct Movement
{
	vec3 forward;
	vec3 velocity;

	Movement(vec3 _forward, vec3 _velocity) 
		: forward(_forward)
		, velocity(_velocity)
	{
		
	}
};