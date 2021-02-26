#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

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

			// todo:
			auto position = transform.position + movement.forward * movement.velocity.z * dt;
			auto v = movement.forward * movement.velocity * dt;

			if (position != transform.position || movement.forward != transform.forward)
			{
				INFO("movment {} {} {} {} {} {} ", v.x, v.y, v.z, movement.forward.x, movement.forward.y, movement.forward.z);
				INFO("velocity {} {} {} {} {} {} ", v.x, v.y, v.z, movement.velocity.x, movement.velocity.y, movement.velocity.z);
				registry.replace<Transform>(e, position, movement.forward);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
