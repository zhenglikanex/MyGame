#pragma once

#include <cstdint>

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/PhysicsActor.hpp"
#include "Framework/Game/Component/PhysicsWorld.hpp"

#include "Framework/Game/Utility/PhysicsUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

namespace rollback_utility
{

	inline bool CheckRollback(Registry& registry, uint32_t verified_frame)
	{
		auto& game_state = registry.ctx<GameState>();
		return  game_state.run_frame > verified_frame && game_state.run_frame - verified_frame <= GameConfig::kMaxRollbackFrameNum;
	}

	inline void RollbackTransform(uint32_t frame)
	{
		
	}

	inline void RollbackPhysicActor(Registry& registry,uint32_t frame)
	{
		auto view = registry.view<TransformState, PhysicsActor>();
		for (auto e : view)
		{
			auto& [state, actor] = view.get<TransformState, PhysicsActor>(e);
			PhysicsUtility::UpdateActorPose(actor.actor, state.value[frame % state.value.size()]);
		}
		//auto& physics_world = registry.ctx<PhysicsWorld>();
		//physics_world.scene->flushQueryUpdates
	}

	inline void RollbackGameplayState(uint32_t frame)
	{

	}
};