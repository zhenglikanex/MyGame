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
		registry.view<Movementcommand, Movement, Transform>().each(
			[=](auto e, const Movementcommand& command, const Movement& movement, const Transform& old_trans)
			{
				//Transform transform;

				////transform.forward = glm::normalize(vec3(command.x_axis, 0, command.y_axis));
				//decltype(Movement::root_motions)::const_iterator iter = std::find_if(
				//	movement.root_motions.cbegin(), 
				//	movement.root_motions.cend(),
				//	[&](const auto& value)
				//	{
				//		auto axis = fpm::abs(fixed16(command.y_axis));
				//		if (axis == value.first)
				//		{
				//			return true;
				//		}
				//		return axis > value.first && axis - value.first < fixed16(0.1f);
				//	});

				//INFO("y_axis:{},delate_position x:{} y:{} z:{}", static_cast<float>(command.y_axis),static_cast<float>(iter->second.delta_position.x), static_cast<float>(iter->second.delta_position.y), static_cast<float>(iter->second.delta_position.z));
				//if (iter != movement.root_motions.end())
				//{
				//	auto delta_position = iter->second.delta_position * (dt / fixed16(0.33));
				//	transform.position = old_trans.position + delta_position;
				//	registry.replace<Transform>(e, transform);

				//	registry.remove<Movementcommand>(e);
				//}
			});
	}

	void Finalize() override
	{
		
	}
};
