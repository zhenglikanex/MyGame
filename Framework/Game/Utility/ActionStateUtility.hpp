#pragma once

#include "Framework/Game/Component/ActionState.hpp"

#include "entt/entt.hpp"

namespace ActionStateUtility
{
	inline void ChangeState(entt::registry& registry, entt::entity e, ActionStateType state)
	{
		if (registry.has<ActionState>(e))
		{
			auto action_state = registry.get<ActionState>(e);
			ChangeState(registry, e, action_state, state);
		}
	}

	inline void ChangeState(entt::registry& registry, entt::entity e, const ActionState& action_state, ActionStateType state)
	{
		if (action_state.cur_state != state)
		{
			registry.emplace_or_replace<ExitActionState>(e, action_state.cur_state);
			registry.emplace_or_replace<EnterActionState>(e, state);
		}	
	}
};