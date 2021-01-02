#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
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
		registry.view<MovementCommond,Movement,Transform>().each([](auto e,const MovementCommond& commond, const Movement& movement,Transform& position)
		{
				vec3 value = vec3(fixed16(0.1));
				position.forward = glm::normalize(vec3(commond.x_axis, 0, commond.y_axis));

				position.pos += movement.velocity * position.forward;
		});
	}

	void Finalize() override
	{
		
	}
};
