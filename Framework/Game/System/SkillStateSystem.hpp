#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/SkillState.hpp"

struct SkillStateSystem : System
{
	SkillStateSystem(entt::registry& registry) : System(registry)
	{

	}

	void Update(fixed16 dt)
	{
		auto view = registry.view<SkillCommand, SkillState, SkillStateMachine>();
		for (auto e : view)
		{
			auto& command = view.get<SkillCommand>(e);
			auto& state = view.get<SkillState>(e);
			auto& skill_state_matchine = view.get<SkillStateMachine>(e);
			
		}

		
	}

	void OnTransition(fixed16 dt)
	{

	}

	void OnExit()
	{

	}

	void OnEnter()
	{

	}

	void ProcessCurState()
	{

	}
};