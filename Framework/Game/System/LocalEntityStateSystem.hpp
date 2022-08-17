#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/LocalEntityState.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

struct LocalEntityStateSystem : public System
{
	explicit LocalEntityStateSystem(Registry& _registry)
		: System(_registry)
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& state = registry.ctx_or_set<LocalEntityState>();
		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			state.identifiers[registry.ctx<GameState>().run_frame % state.identifiers.size()] = item.second.identifier;
			state.nums[registry.ctx<GameState>().run_frame % state.nums.size()] = registry.view<Local>().size();
		}
	}

	void Finalize() override
	{

	}
};