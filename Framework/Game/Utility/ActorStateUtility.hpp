#pragma once

#include "Framework/Game/Component/ActorState.hpp"

#include "entt/entt.hpp"

namespace ActionStateUtility
{
	inline void ChangeState(entt::registry& registry, entt::entity e, const ActorState& action_state, ActorStateType state)
	{
		if (action_state.cur_state != state)
		{
			registry.emplace_or_replace<ExitActorState>(e, action_state.cur_state);
			registry.emplace_or_replace<EnterActorState>(e, state);
		}
	}

	inline void ChangeState(entt::registry& registry, entt::entity e, ActorStateType state)
	{
		if (registry.has<ActorState>(e))
		{
			auto action_state = registry.get<ActorState>(e);
			ChangeState(registry, e, action_state, state);
		}
		else
		{
			registry.emplace_or_replace<EnterActorState>(e, state);
		}
	}
};