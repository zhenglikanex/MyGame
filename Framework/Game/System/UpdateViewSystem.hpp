#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"

struct UpdateViewSystem : public System
{
	entt::observer observer{ registry, entt::collector.update<Transform>().where<View>() };

	UpdateViewSystem(entt::registry& _registry) : System(_registry) { }
	~UpdateViewSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		for (auto e : observer)
		{
			auto& view = registry.get<View>(e);
			const auto& transform = registry.get<Transform>(e);
			
			view.value->MovePosition(transform.position);
			view.value->MoveForward(transform.forward);
		}

		observer.clear();
	}

	void Finalize() override
	{
	}
};