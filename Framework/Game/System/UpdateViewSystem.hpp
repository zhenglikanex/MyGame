#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"

struct UpdateViewSystem : public System
{
	UpdateViewSystem(entt::registry& _registry) : System(_registry) { }
	~UpdateViewSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		registry.view<const View, const Transform>().each([](auto, const View& view, const Transform& position) 
		{
			//view.value->Move(position.pos);
			//view.value->SetForward(position.forward);
		});

		// todo animation
	}

	void Finalize() override
	{
	}
};