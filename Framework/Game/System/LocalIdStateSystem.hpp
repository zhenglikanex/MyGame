#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/LocalIdState.hpp"
#include "Framework/Game/Component/PlayerManager.hpp"

struct LocalIdStateSystem : public System
{
	LocalIdStateSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~LocalIdStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& state = registry.ctx<LocalIdState>();
		auto& player_manager = registry.ctx<PlayerManager>();
		for (auto& item : player_manager.players)
		{
			INFO("frame {} id {}", registry.ctx<GameState>().run_frame, item.second.local_id_generator.value);
			state.value[registry.ctx<GameState>().run_frame % state.value.size()] = item.second.local_id_generator;
		}
		
	}

	void Finalize() override
	{

	}
};