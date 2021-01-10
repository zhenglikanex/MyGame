#include <algorithm>

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/State.hpp"

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
		registry.view<MovementCommond, Movement, Transform>(entt::exclude<HurtState, AttackState, DeathState>).each(
			[=](auto e, const MovementCommond& commond, const Movement& movement, const Transform& old_trans)
			{
				Transform transform;

				//transform.forward = glm::normalize(vec3(commond.x_axis, 0, commond.y_axis));
				decltype(Movement::root_motions)::const_iterator iter = std::find_if(
					movement.root_motions.cbegin(), 
					movement.root_motions.cend(),
					[&](const auto& value)
					{
						return fixed16(commond.y_axis) - value.first <= fixed16(0.1f);
					});


				INFO("y_axis:{} , delate_position x:{} y:{} z:{}", static_cast<float>(commond.y_axis),static_cast<float>(iter->second.delta_position.x), static_cast<float>(iter->second.delta_position.y), static_cast<float>(iter->second.delta_position.z));

				auto delta_position = iter->second.delta_position * (dt / fixed16(0.33));
				transform.position = old_trans.position + delta_position;

				registry.replace<Transform>(e, transform);
				
				registry.remove<MovementCommond>(e);
			});
	}

	void Finalize() override
	{
		
	}
};
