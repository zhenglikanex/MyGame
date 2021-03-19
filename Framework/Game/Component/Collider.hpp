#pragma once

#include "Framework/Game/Collision.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct Collider
{
	Geometry geometry;
	bool trigger;

	entt::entity owner;

	Collider(const Geometry& _gemoetry, bool _trigger,entt::entity _owner)
		: geometry(_gemoetry)
		, trigger(_trigger)
		, owner(_owner)
	{

	}
};