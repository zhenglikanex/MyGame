#pragma once

#include "Framework/Game/System.hpp"

struct CollisionSystem : public System
{
	CollisionSystem(entt::registry& _registry) : System(_registry) { }
	~CollisionSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{

	}

	void Finalize() override
	{

	}
};
