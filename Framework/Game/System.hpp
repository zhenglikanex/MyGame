#pragma once

#include <type_traits>

#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Registry.hpp"

#include "entt/entt.hpp"

struct System
{
	System(Registry& _registry) 
		: registry(_registry) 
	{ }

	virtual ~System() = 0 {}

	virtual bool Initialize() = 0;
	virtual void Update(float dt) {}
	virtual void FixedUpdate(float dt) {}
	virtual void LateUpdate(float dt) {}
	virtual void Finalize() = 0;

	Registry& registry;
};