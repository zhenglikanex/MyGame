#pragma once

#include <algorithm>

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"

struct MovementSystem : public System
{
	MovementSystem(entt::registry& _registry) : System(_registry) { }
	~MovementSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<Movement, Transform>();
		for (auto e : view)
		{
			const auto& movement = view.get<Movement>(e);
			auto& transform = view.get<Transform>(e);

			//
		}
	}

	void Finalize() override
	{
		
	}
};
