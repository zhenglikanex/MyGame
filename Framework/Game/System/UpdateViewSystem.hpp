#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Position.hpp"

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
		registry.view<const View, const Position>().each([](auto, const View& view, const Position& position) 
		{
			view.value->SetPosition(position.pos);
			view.value->SetForward(position.forward);
		});

		// todo animation
	}

	void Finalize() override
	{
	}
};