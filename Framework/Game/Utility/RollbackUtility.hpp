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

struct RollbackUtility
{
public:
	static bool CheckRollback(entt::registry& registry, uint32_t frame)
	{
		auto& game_state = registry.ctx<GameState>();
		return game_state.run_frame - frame <= GameConfig::kMaxRollbackFrameNum;
	}

	static void RollbackTransform(uint32_t frame)
	{
		
	}

	static void RollbackPhysicActor(entt::registry& registry,uint32_t frame)
	{
		auto view = registry.view<TransformState, PhysicsActor>();
		for (auto e : view)
		{
			auto& [state, actor] = view.get<TransformState, PhysicsActor>(e);
			PhysicsUtility::UpdateActorPose(actor.actor, state.value[frame]);
		}
		//auto& physics_world = registry.ctx<PhysicsWorld>();
		//physics_world.scene->flushQueryUpdates
	}

	static void RollbackGameplayState(uint32_t frame)
	{

	}
};