#pragma once

#include <type_traits>
#include "Framework/Game/DebugService.hpp"

#include "FixedPoint.hpp"

#include "entt/entt.hpp"

struct System
{
	System(entt::registry& _registry) : registry(_registry) { }
	virtual ~System() = 0 {}

	virtual bool Initialize() = 0;
	virtual void Update(fixed16 dt) {}
	virtual void FixedUpdate(fixed16 dt) {}
	virtual void LateUpdate(fixed16 dt) {}
	virtual void Finalize() = 0;

	entt::registry& registry;
};