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
	struct State
	{
		entt::registry& registry;
		
		State(entt::registry& _registry) : registry(_registry) {}
		virtual ~State() = 0 {}

		virtual ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) = 0;
		virtual void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) = 0;
		virtual void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) = 0;
		virtual void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) = 0;
	};

	struct IdleState : State
	{
		IdleState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			if (command.attack)
			{
				return ActionStateType::kAttack;
			}

			if (command.x_axis != fixed16(0) || command.y_axis != fixed16(0))
			{
				return ActionStateType::kMovement;
			}

			if (command.jump)
			{
				return ActionStateType::kJump;
			}

			return ActionStateType::kIdle;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			
		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			
		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}
	};

	struct MovementState : State
	{
		MovementState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			if (command.attack)
			{
				return ActionStateType::kAttack;
			}

			if (command.x_axis <= fixed16(0) || command.y_axis <= fixed16(0))
			{
				return ActionStateType::kIdle;
			}

			return ActionStateType::kMovement;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			//animation.id = "idle";

		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			// ÒÆ¶¯
			auto root_motions = registry.ctx<RootMotionConfig>().GetEntry("Locomotion");
			auto iter = root_motions.find(fpm::abs(fpm::round(command.y_axis)));
			if (iter != root_motions.cend())
			{
				registry.emplace_or_replace<Movement>(e, vec3(command.x_axis, 0, command.y_axis), iter->second.delta_position);
			}
		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			
		}
	};

	struct JumpState : State
	{
		JumpState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			return ActionStateType::kJump;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}
	};

	struct AttackState : State
	{
		AttackState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			if (animation.is_done)
			{
				return ActionStateType::kIdle;
			}

			return ActionStateType::kAttack;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			
		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}
	};

	struct HurtState : State
	{
		HurtState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			if (animation.is_done)
			{
				return ActionStateType::kIdle;
			}

			return ActionStateType::kHurt;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}
	};

	struct DeathState : State
	{
		DeathState(entt::registry& _registry) : State(_registry) {}

		ActionStateType InputCommand(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{
			return ActionStateType::kDeath;
		}

		void OnEnter(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnUpdate(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}

		void OnExit(entt::entity e, const Command& command, const ActionState& action_state, Animation& animation) override
		{

		}
	};

	std::array<std::unique_ptr<State>, (size_t)ActionStateType::kMax> states;

	CommandProcessSystem(entt::registry& _registry)
		: System(_registry)
		, states({
			std::make_unique<IdleState>(registry),
			std::make_unique<MovementState>(registry),
			std::make_unique<JumpState>(registry),
			std::make_unique<AttackState>(registry),
			std::make_unique<HurtState>(registry),
			std::make_unique<DeathState>(registry) })
	{ }
	~CommandProcessSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		const auto& command_group = registry.ctx<commandGroup>();
		for (auto e : registry.view<Command, ActionState, Animation>())
		{
			const auto& command = registry.get<Command>(e);
			auto& action_state = registry.get<ActionState>(e);
			auto& animation = registry.get<Animation>(e);
			
			auto new_animation = animation;

			auto& executor = states[(size_t)action_state.cur_state];
			if (action_state.time <= fixed16(0))
			{
				executor->OnEnter(e, command, action_state, animation);
			}

			auto state_type = executor->InputCommand(e, command, action_state, new_animation);
			if (state_type != action_state.cur_state)
			{
				executor->OnExit(e, command, action_state, new_animation);
				action_state.cur_state = state_type;
				action_state.time = fixed16(0);

				executor->OnEnter(e, command, action_state, new_animation);
			}

			executor->OnUpdate(e, command, action_state, new_animation);

			if (new_animation.name != animation.name)
			{
				registry.replace<Animation>(e,new_animation);
			}
		}
	}

	void Finalize() override
	{

	}
};
