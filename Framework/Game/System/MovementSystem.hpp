#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/Local.hpp"

#include "Framework/Game/System.hpp"

#include "PxPhysicsAPI.h"

inline quat ForwardRotation(const vec3& forward)
{
	if (forward.x >= 0.0)
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), -1.f, 1.f);
		return quat(vec3(0, std::acos(cos), 0));
	}
	else
	{
		auto cos = glm::clamp(glm::dot(forward, vec3(0.0, 0.0, 1.0)), -1.f, 1.f);
		return quat(vec3(0, -std::acos(cos), 0));
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
			auto position = transform.position + movement.forward * movement.velocity.z * static_cast<float>(dt);
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
