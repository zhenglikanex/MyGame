#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/Local.hpp"

struct TransformStateSystem : public System
{
	TransformStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~TransformStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto run_frame = registry.ctx<GameState>().run_frame;
		auto view = registry.view<Transform, TransformState,Local>();
		for (auto e : view)
		{
			auto&[transform, transform_state] = view.get<Transform, TransformState>(e);
			transform_state.value[run_frame % transform_state.value.size()] = transform;
		}
	}

	void Finalize() override
	{

	}
};