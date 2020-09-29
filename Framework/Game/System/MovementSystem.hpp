#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Component/Position.hpp"
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
		registry.view<MovementCommond,Movement,Position>().each([](auto e,const MovementCommond& commond, const Movement& movement,Position& position)
		{
			position.forward = glm::normalize(vec3(commond.x_axis, 0, commond.y_axis));
			position.pos += movement.velocity * position.forward;
		});
	}

	void Finalize() override
	{
		
	}
};
