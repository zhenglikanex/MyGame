#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/SyncInstanceState.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

struct SyncInstanceStateSystem : public System
{
	SyncInstanceStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~SyncInstanceStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& state = registry.ctx<SyncInstanceState>();
		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			state.value[registry.ctx<GameState>().run_frame % state.value.size()].identifier = item.second.identifier;
		}
	}

	void Finalize() override
	{

	}
};