#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/SyncDataState.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

struct SyncDataStateSystem : public System
{
	SyncDataStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~SyncDataStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& state = registry.ctx<SyncDataState>();
		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_datas)
		{
			state.value[registry.ctx<GameState>().run_frame % state.value.size()] = item.second.identifier;
		}
	}

	void Finalize() override
	{

	}
};