#pragma once

#include <algorithm>

#include "Framework/Game/LogService.hpp"

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
			auto delt = movement.velocity * dt;
			auto new_pos = transform.position + movement.velocity * dt;
			auto new_forward = transform.forward;

			if (new_pos != transform.position || new_forward != transform.forward)
			{
				auto& command = registry.get<Command>(e);
				auto& clip = registry.get<AnimationClip>(e);
				INFO("c++ {:.1f} time {} velocity x:{} y:{} z:{} position x:{} y:{} z:{}", command.y_axis,clip.time, movement.velocity.x, movement.velocity.y, movement.velocity.z, delt.x, delt.y, delt.z);

				registry.replace<Transform>(e, new_pos, new_forward);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
