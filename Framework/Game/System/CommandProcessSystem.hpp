#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ActionState.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/RootMotionConfig.hpp"


struct CommandProcessSystem : public System
{
	CommandProcessSystem(entt::registry& _registry)
		: System(_registry)
	{ 
	}

	~CommandProcessSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		for (auto e : registry.view<Command, ActionState, Animation>())
		{
			const auto& command = registry.get<Command>(e);
			const auto& action_state = registry.get<ActionState>(e);

			if (IsCanMovement(action_state.cur_state))
			{
				auto root_motions = registry.ctx<RootMotionConfig>().GetEntry("Locomotion");
				auto iter = root_motions.find(fpm::abs(fpm::round(command.y_axis)));
				if (iter != root_motions.cend())
				{
					registry.emplace_or_replace<Movement>(e, vec3(command.x_axis, 0, command.y_axis), iter->second.delta_position);
				}
			}

			if (IsCanAttack(action_state.cur_state))
			{

			}

			if (IsCanJump(action_state.cur_state))
			{

			}
		}
	}

	bool IsCanMovement(ActionStateType state)
	{
		return state == ActionStateType::kMovement;
	}

	bool IsCanAttack(ActionStateType state)
	{
		return state == ActionStateType::kAttack;
	}

	bool IsCanJump(ActionStateType state)
	{
		return state == ActionStateType::kJump;
	}

	void Finalize() override
	{

	}
};
