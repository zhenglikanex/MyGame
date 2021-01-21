#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ActionState.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActionStateUtility.hpp"

#include "Framework/Game/Data/RootMotionConfig.hpp"

struct ActionStateSystem : public System
{
	struct State
	{
		entt::registry& registry;

		State(entt::registry& _registry) : registry(_registry) {}
		virtual ~State() = 0 {}

		virtual ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) 
		{
			return action_state.cur_state; 
		}

		virtual void OnEnter(entt::entity e, const EnterActionState& action_state) { }
		virtual void OnExit(entt::entity e, const ExitActionState& action_state) { }
	};

	struct IdleState : State
	{
		IdleState(entt::registry& _registry) : State(_registry) {}

		ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) override
		{
			if (command.x_axis > fixed16(0) || command.y_axis > fixed16(0))
			{
				registry.emplace_or_replace<ExitActionState>(e, ActionStateType::kIdle);
				registry.emplace_or_replace<EnterActionState>(e, ActionStateType::kMovement);
				return;
			}
		}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}
	};

	struct MovementState : State
	{
		MovementState(entt::registry& _registry) : State(_registry) {}

		ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) override
		{
			if (command.attack)
			{
				return ActionStateType::kAttack;
			}

			if (command.jump)
			{
				return ActionStateType::kJump;
			}

			if (command.x_axis <= fixed16(0) && command.y_axis <= fixed16(0))
			{	
				return ActionStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}
	};

	struct JumpState : State
	{
		JumpState(entt::registry& _registry) : State(_registry) {}

		ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActionStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

	};

	struct AttackState : State
	{
		AttackState(entt::registry& _registry) : State(_registry) {}

		ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActionStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}
	};

	struct HurtState : State
	{
		HurtState(entt::registry& _registry) : State(_registry) {}

		ActionStateType OnTransition(entt::entity e, const ActionState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActionStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}
	};

	struct DeathState : State
	{
		DeathState(entt::registry& _registry) : State(_registry) {}

		void OnEnter(entt::entity e, const EnterActionState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}
	};

	std::array<std::unique_ptr<State>, (size_t)ActionStateType::kMax> states;

	ActionStateSystem(entt::registry& _registry)
		: System(_registry)
		, states({
			std::make_unique<IdleState>(registry),
			std::make_unique<MovementState>(registry),
			std::make_unique<JumpState>(registry),
			std::make_unique<AttackState>(registry),
			std::make_unique<HurtState>(registry),
			std::make_unique<DeathState>(registry) })
	{ }
	~ActionStateSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		OnUpdate(dt);
		OnExit();
		OnEnter();
	}

	void OnUpdate(fixed16 dt)
	{
		for (auto e : registry.view<ActionState, Animation, Command>())
		{
			auto& action_state = registry.get<ActionState>(e);
			const auto& animation = registry.get<Animation>(e);
			const auto& command = registry.get<Command>(e);

			action_state.time += dt;

			auto& executor = states[(size_t)action_state.cur_state];
			auto next_state = executor->OnTransition(e, action_state, animation, command);
			ActionStateUtility::ChangeState(registry, e, next_state);
		}
	}

	void OnExit()
	{
		for (auto e : registry.view<ExitActionState>())
		{
			auto& exit_state = registry.get<ExitActionState>(e);

			auto& executor = states[(size_t)exit_state.value];
			executor->OnExit(e,exit_state);
		}
	}

	void OnEnter()
	{
		for (auto e : registry.view<EnterActionState,Animation>())
		{
			auto& enter_state = registry.get<EnterActionState>(e);
			
			auto& executor = states[(size_t)enter_state.value];
			executor->OnEnter(e,enter_state);

			registry.emplace_or_replace<ActionState>(e, enter_state.value);
		}
	}

	void LateUpdate(fixed16 dt) override
	{
		registry.clear<EnterActionState>();
		registry.clear<ExitActionState>();
	}

	void Finalize() override
	{
		
	}
};
