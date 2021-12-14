#pragma once

#include "Framework/Game/Component/Destory.hpp"

#include "Framework/Game/System.hpp"


struct DestorySystem : System
{
public:
	DestorySystem(entt::registry& _registry) : System(_registry)
	{
	}
	~DestorySystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<Destory>();
		registry.destroy(view.begin(), view.end());
	}

	void Finalize() override
	{

	}
};