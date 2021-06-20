#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/Local.hpp"

#include "Framework/Game/System.hpp"

inline quat ForwardRotation(const vec3& forward)
{
	if (forward.x >= fixed16(0))
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), fixed16(-1), fixed16(1));
		return quat(vec3(0, fpm::acos(cos), 0));
	}
	else
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), fixed16(-1), fixed16(1));
		return quat(vec3(0, -fpm::acos(cos), 0));
	}
}

struct MovementSystem : public System
{
	MovementSystem(entt::registry& _registry) : System(_registry) { }
	~MovementSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<Local, Movement, Transform>();
		for (auto e : view)
		{
			const auto& movement = view.get<Movement>(e);
			auto& transform = view.get<Transform>(e);

			// todo:
			auto position = transform.position + movement.forward * movement.velocity.z * dt;
			auto rotation = ForwardRotation(movement.forward);

			if (position != transform.position || rotation != transform.rotation)
			{	
				registry.replace<Transform>(e, position, rotation);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
