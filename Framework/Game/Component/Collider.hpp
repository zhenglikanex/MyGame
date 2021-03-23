#pragma once

#include "Framework/Game/Collision.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct Collider
{
	Geometry geometry;
	bool trigger;

	entt::entity owner;

	Collider() = default;

	Collider(const Geometry& _gemoetry, bool _trigger,entt::entity _owner)
		: geometry(_gemoetry)
		, trigger(_trigger)
		, owner(_owner)
	{

	}
};

NON_MEMEBER_BAR(Collider, obj.geometry, obj.trigger, obj.owner);