#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

struct GameplayStateSystem : public System
{
	GameplayStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~GameplayStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto run_frame = registry.ctx<GameState>().run_frame;
		auto view = registry.view<GameplayState,Local>();
		for (auto e : view)
		{
			auto&[gameplay_state] = view.get<GameplayState>(e);
		}
	}

	void Finalize() override
	{

	}
};