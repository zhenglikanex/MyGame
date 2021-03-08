#pragma once

#include <algorithm>

#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

inline quat ForwardRotation(const vec3& forward)
{
	if (forward.x > fixed16(0) && forward.z >= fixed16(0))
	{
		if (forward.z == fixed16(0))
		{
			return quat(vec3(0, glm::radians(fixed16(90)), 0));
		}
		
		return quat(vec3(0, fpm::acos(glm::dot(forward, vec3(0.0, 0.0, 1.0))), 0));
	}
	else if (forward.x < fixed16(0) && forward.z >= fixed16(0))
	{
		if (forward.z == fixed16(0))
		{
			return quat(vec3(0, -glm::radians(fixed16(90)), 0));
		}

		return quat(vec3(0, -fpm::acos(glm::dot(forward, vec3(0.0, 0.0, 1.0))), 0));
	}
	else if (forward.x <= fixed16(0) && forward.z < fixed16(0))
	{
		if (forward.x == fixed16(0))
		{
			quat(vec3(0, glm::radians(fixed16(-180)), 0));
		}

		return quat(vec3(0, -fpm::acos(glm::dot(forward, vec3(0.0, 0.0, 1.0))) - fixed16(90), 0));
	}
	else if (forward.x >= fixed16(0) && forward.z < fixed16(0))
	{
		if (forward.x == fixed16(0))
		{
			quat(vec3(0, 0, 0));
		}

		return quat(vec3(0, fpm::acos(glm::dot(forward, vec3(0.0, 0.0, 1.0))) + fixed16(90), 0));
	}
	else
	{
		return quat(fixed16(0), fixed16(0), fixed16(0), fixed16(1));
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

	void Update(fixed16 dt) override
	{
		auto view = registry.view<Movement, Transform>();
		for (auto e : view)
		{
			const auto& movement = view.get<Movement>(e);
			auto& transform = view.get<Transform>(e);

			// todo:
			auto position = transform.position + movement.forward * movement.velocity.z * dt;
			auto rotation = ForwardRotation(movement.forward);
			auto v = movement.forward * movement.velocity * dt;

			if (position != transform.position || rotation != transform.rotation)
			{
				INFO("movment {} {} {} {} {} {} ", v.x, v.y, v.z, movement.forward.x, movement.forward.y, movement.forward.z);
				INFO("velocity {} {} {} {} {} {} ", v.x, v.y, v.z, movement.velocity.x, movement.velocity.y, movement.velocity.z);
				
				registry.replace<Transform>(e, position, rotation);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
