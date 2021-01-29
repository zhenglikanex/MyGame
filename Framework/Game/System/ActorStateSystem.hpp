#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

#include "Framework/Game/Fmt.hpp"

/*
 * ״̬������һ֡����ʵ��
 * OnEnter�¼��в��ܽ���״̬�л�
 */

struct ActorStateSystem : public System
{
	struct State
	{
		entt::registry& registry;

		State(entt::registry& _registry) : registry(_registry) {}
		virtual ~State() = 0 {}

		virtual ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const AnimationClip& animation_clip, const Command& command)
		{
			return action_state.cur_state;
		}

		virtual void OnEnter(entt::entity e, const EnterActorState& action_state) { }
		virtual void OnExit(entt::entity e, const ExitActorState& action_state) { }
		virtual void OnUpdate(entt::entity e,const Command& command,const AnimationClip& animction_clip) { }
	};

	struct IdleState : State
	{
		IdleState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const AnimationClip& animation, const Command& command) override
		{
			if (command.x_axis > fixed16(0) || command.y_axis > fixed16(0))
			{
				return ActorStateType::kMovement;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override { }
	};

	struct MovementState : State
	{
		MovementState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const AnimationClip& animation, const Command& command) override
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
			
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override
		{
			// �ƶ�����ͨ��������ϵõ�,���ද�����ݵ�ʱ�򽫻�ϲ�����Ϊkey����������ɢ�Ķ�������
			auto axis = fpm::floor(command.y_axis * 10) / 10;
			std::string name = fmt::format("{}{}", GameConfig::ActionAnimation::kMovement, axis.raw_value());
			if (name == animction_clip.name)
			{
				return;
			}

			registry.emplace_or_replace<AnimationClip>(e, name);
		}
	};

	struct JumpState : State
	{
		JumpState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const AnimationClip& animtion_clip, const Command& command) override
		{
			if (animtion_clip.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override { }
	};

	struct AttackState : State
	{
		AttackState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state,  const AnimationClip& animtion_clip, const Command& command) override
		{
			if (animtion_clip.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override { }
	};

	struct HurtState : State
	{
		HurtState(entt::registry& _registry) : State(_registry) {}

		ActorStateType OnTransition(entt::entity e, const ActorState& action_state, const AnimationClip& animtion_clip, const Command& command) override
		{
			if (animtion_clip.is_done)
			{
				return ActorStateType::kIdle;
			}

			return action_state.cur_state;
		}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override { }
	};

	struct DeathState : State
	{
		DeathState(entt::registry& _registry) : State(_registry) {}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}

		void OnUpdate(entt::entity e, const Command& command,const AnimationClip& animction_clip) override { }
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

		ProcessCurState();
	}

	void OnTransition(fixed16 dt)
	{
		auto view = registry.view<ActorState,AnimationClip, Command>(entt::exclude<EnterActorState, ExitActorState>);
		for (auto e : view)
		{
			auto& action_state = view.get<ActorState>(e);
			const auto& animation_clip = view.get<AnimationClip>(e);
			const auto& command = view.get<Command>(e);

			action_state.time += dt;

			auto& executor = states[(size_t)action_state.cur_state];
			auto next_state = executor->OnTransition(e, action_state,animation_clip, command);
			ActionStateUtility::ChangeState(registry, e, next_state);
		}
	}

	void OnExit()
	{
		auto view = registry.view<ExitActorState>();
		for (auto e : view)
		{
			auto& exit_state = view.get<ExitActorState>(e);

			auto& executor = states[(size_t)exit_state.value];
			executor->OnExit(e, exit_state);
		}
	}

	void OnEnter()
	{
		auto view = registry.view<EnterActorState>();
		for (auto e : view)
		{
			auto& enter_state = view.get<EnterActorState>(e);
			
			auto& executor = states[(size_t)enter_state.value];
			executor->OnEnter(e,enter_state);

			registry.emplace_or_replace<ActorState>(e, enter_state.value);
		}
	}

	void ProcessCurState()
	{
		auto view = registry.view<ActorState, Command, AnimationClip>();
		for (auto e : view)
		{
			auto& action_state = view.get<ActorState>(e);
			auto& command = view.get<Command>(e);
			auto& animction_clip = view.get<AnimationClip>(e);

			auto& executor = states[(size_t)action_state.cur_state];
			executor->OnUpdate(e, command,animction_clip);
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