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
	explicit MovementSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Local, MovementForward, Movement, Transform>();
		for (auto e : view)
		{
			const auto& forward = view.get<MovementForward>(e);
			const auto& movement = view.get<Movement>(e);
			auto& transform = view.get<Transform>(e);

			// todo:
			auto position = transform.position + forward.value * movement.velocity.z * dt;
			auto rotation = ForwardRotation(forward.value);

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
