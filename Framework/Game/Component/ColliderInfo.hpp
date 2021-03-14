#pragma once

#include "Framework/Game/Geometry.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct ColliderInfo
{
	Geometry geometry;
	mat4 transform;

	entt::entity collider;
};

