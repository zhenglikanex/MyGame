#pragma once

#include "FixedPoint.hpp"

#include "entt/entt.hpp"

struct System
{
	System(entt::registry& _registry) : registry(_registry) { }
	virtual ~System() = 0 {}

	virtual bool Initialize() = 0;
	virtual void Update(fixed16 dt) = 0;
	virtual void Finalize() = 0;

	entt::registry& registry;
};