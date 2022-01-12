#pragma once

#include <variant>

#include "3rdparty/include/entt/entt.hpp"

#include "PxPhysicsAPI.h"

enum class HitType : uint8_t
{
	kRay,
	kOverlap,
	kSweep
};

struct Hit
{
	HitType hit_type;
	entt::entity source;
	std::variant<physx::PxSphereGeometry, physx::PxBoxGeometry> geometry;
};