#pragma once

#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "Framework/Game/System.hpp"


struct DestroyModifierAttributeSystem : System
{
public:
	DestroyModifierAttributeSystem(Registry& _registry) : System(_registry)
	{
	}
	~DestroyModifierAttributeSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<ModifierAttribute>();
		registry.destroy(view.begin(), view.end());
	}

	void Finalize() override
	{

	}
};