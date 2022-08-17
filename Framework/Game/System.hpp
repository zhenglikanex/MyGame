#pragma once

#include <type_traits>

#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Registry.hpp"

struct System
{
	Registry& registry;

	System(Registry& _registry) 
		: registry(_registry) 
	{ }

	System(const System&) = delete;
	System(System&&) = delete;
	System& operator=(const System&) = delete;
	System& operator=(System&&) = delete;

	virtual ~System() = 0 {}

	virtual bool Initialize() = 0;
	virtual void Update(float dt) {}
	virtual void FixedUpdate(float dt) {}
	virtual void LateUpdate(float dt) {}
	virtual void Finalize() = 0;
};
