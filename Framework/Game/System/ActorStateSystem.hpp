#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/Asset.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

/*
 * 状态会在下一帧进行实际
 * OnEnter事件中不能进行状态切换
 */

struct ActorStateSystem : public System
{
	struct State
	{
		entt::registry& registry;

		State(entt::registry& _registry) : registry(_registry) {}
		virtual ~State() = 0 {}

		virtual ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) 
		{
			return action_state.cur_state; 
		}

		virtual void OnEnter(entt::entity e, const EnterActorState& action_state) { }
		virtual void OnExit(entt::entity e, const ExitActorState& action_state) { }
		virtual void OnUpdate(entt::entity e,const Command& command) { }
	};

	struct IdleState : State
	{
		IdleState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) override
		{
			if (command.x_axis > fixed16(0) || command.y_axis > fixed16(0))
			{
				return ActorStateType::kMovement;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	struct MovementState : State
	{
		MovementState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) override
		{
			if (command.attack)
			{
				return ActorStateType::kAttack;
			}

			if (command.jump)
			{
				return ActorStateType::kJump;
			}

			if (command.x_axis <= fixed16(0) && command.y_axis <= fixed16(0))
			{	
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	struct JumpState : State
	{
		JumpState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	struct AttackState : State
	{
		AttackState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	struct HurtState : State
	{
		HurtState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const Animation& animation, const Command& command) override
		{
			if (animation.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	struct DeathState : State
	{
		DeathState(entt::registry& _registry) : State(_registry) {}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<Animation>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command) override { }
	};

	std::array<std::unique_ptr<State>, (size_t)ActorStateType::kMax> states;

	ActorStateSystem(entt::registry& _registry)
		: System(_registry)
		, states({
			std::make_unique<IdleState>(registry),
			std::make_unique<MovementState>(registry),
			std::make_unique<JumpState>(registry),
			std::make_unique<AttackState>(registry),
			std::make_unique<HurtState>(registry),
			std::make_unique<DeathState>(registry) })
	{ }
	~ActorStateSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		OnTransition(dt);
		OnExit();
		OnEnter();
	}

	void OnTransition(fixed16 dt)
	{
		for (auto e : registry.view<ActorState, Animation, Command>(entt::exclude_t<EnterActorState, ExitActorState>{}))
		{
			auto& action_state = registry.get<ActorState>(e);
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
		for (auto e : registry.view<ExitActorState>())
		{
			auto& exit_state = registry.get<ExitActorState>(e);

			auto& executor = states[(size_t)exit_state.value];
			executor->OnExit(e, exit_state);
		}
	}

	void OnEnter()
	{
		for (auto e : registry.view<EnterActorState>())
		{
			auto& enter_state = registry.get<EnterActorState>(e);
			
			auto& executor = states[(size_t)enter_state.value];
			executor->OnEnter(e,enter_state);

			registry.emplace_or_replace<ActorState>(e, enter_state.value);
		}
	}

	void ProcessCurState()
	{
		for (auto e : registry.view<ActorState,Command>())
		{
			auto& action_state = registry.get<ActorState>(e);
			auto& command = registry.get<Command>(e);
			auto& executor = states[(size_t)action_state.cur_state];
			executor->OnUpdate(e, command);
		}
	}

	void LateUpdate(fixed16 dt) override
	{
		registry.clear<EnterActorState>();
		registry.clear<ExitActorState>();
	}

	void Finalize() override
	{
		
	}
};