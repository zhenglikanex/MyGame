#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/SkillState.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

#include "Framework/Game/Fmt.hpp"

inline vec2 SquareToCircle(const vec2& input)
{
	vec2 output = zero<vec2>();
	
	output.x = input.x * sqrt(fixed16(1) - (input.y * input.y) / fixed16(2.0f));
	output.y = input.y * sqrt(fixed16(1) - (input.x * input.x) / fixed16(2.0f));

	auto x = static_cast<float>(output.x);
	auto y = static_cast<float>(output.y);

	return output;
}

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
			if (command.skill != 0)
			{
				return ActorStateType::kAttack;
			}

			if (command.x_axis != fixed16(0) || command.y_axis != fixed16(0))
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
			if (command.skill != 0)
			{
				return ActorStateType::kAttack;
			}

			if (command.jump)
			{
				return ActorStateType::kJump;
			}

			if (command.x_axis == fixed16(0) && command.y_axis == fixed16(0))
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
			vec2 input = SquareToCircle(vec2(command.x_axis, command.y_axis));

			//旋转
			auto forward = glm::normalize(vec3(input.x, fixed16(0), input.y));
			auto movement = registry.try_get<Movement>(e);
			if (movement)
			{
				movement->forward = forward;
			}
			else
			{
				registry.emplace<Movement>(e, forward, zero<vec3>(), zero<vec3>());
			}

			INFO("input {}", glm::length(input));

			// 移动
			// 移动动画通过动画混合得到,这类动画数据的时候将混合参数作为key导出所有离散的动画数据	
			// todo:存在浮点数问题,这里会四舍五入所有问题应该不是很大？
			std::string name = fmt::format("{}|{:.1f}", GameConfig::ActionAnimation::kLocomotion,glm::length(input));
			if (name == animction_clip.name)
			{
				return;
			}

			auto animation_clip = registry.try_get<AnimationClip>(e);
			if (animation_clip && animation_clip->name.find(GameConfig::ActionAnimation::kLocomotion) != std::string::npos)
			{
				registry.replace<AnimationClip>(e, name,animation_clip->time);
			}
			else
			{
				registry.emplace_or_replace<AnimationClip>(e, name);
			}
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
			// 判断是否通过技能图执行
			auto skill_grpah = registry.try_get<SkillGraph>(e);
			if (skill_grpah != nullptr)
			{
				registry.emplace_or_replace<SkillState>(e, skill_grpah->value->entry());
				auto skill_params =  registry.emplace_or_replace<SkillParams>(e);
				skill_params.value.clear();
				
				SkillGraphInfo::Param skill;
				skill.int_value = 0;
				skill_params.value.emplace("skill", skill);

				SkillGraphInfo::Param state_time;
				skill.float_value = fixed16(0);
				skill_params.value.emplace("state_time", state_time);
			}
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override 
		{
			auto skill_params = registry.try_get<SkillParams>(e);
			if (skill_params != nullptr)
			{
				auto& skill = skill_params->value["skill"];
				if (skill.int_value == 0)
				{
					skill.int_value = command.skill;
				}

				auto& state_time = skill_params->value["state_time"];
				state_time.float_value = animction_clip.time;
			}
		}
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
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kHurt);
		}

		void OnUpdate(entt::entity e, const Command& command, const AnimationClip& animction_clip) override { }
	};

	struct DeathState : State
	{
		DeathState(entt::registry& _registry) : State(_registry) {}

		void OnEnter(entt::entity e, const EnterActorState& action_state) override
		{
			registry.emplace_or_replace<AnimationClip>(e, GameConfig::ActionAnimation::kDeath);
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

	void FixedUpdate(fixed16 dt) override
	{
		OnTransition(dt);
		OnExit();
		OnEnter();

		ProcessCurState(dt);

		registry.clear<EnterActorState>();
		registry.clear<ExitActorState>();
	}

	void OnTransition(fixed16 dt)
	{
		auto view = registry.view<ActorState,AnimationClip, Command>(entt::exclude<EnterActorState, ExitActorState>);
		for (auto e : view)
		{
			auto& action_state = view.get<ActorState>(e);
			const auto& animation_clip = view.get<AnimationClip>(e);
			const auto& command = view.get<Command>(e);

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

	void ProcessCurState(fixed16 dt)
	{
		auto view = registry.view<ActorState, Command, AnimationClip>();
		for (auto e : view)
		{
			auto& action_state = view.get<ActorState>(e);
			auto& command = view.get<Command>(e);
			auto& animction_clip = view.get<AnimationClip>(e);

			action_state.time += dt;

			INFO("process command x {} y {}", command.x_axis, command.y_axis);

			auto& executor = states[(size_t)action_state.cur_state];
			executor->OnUpdate(e, command, animction_clip);
		}
	}

	void LateUpdate(fixed16 dt) override
	{
		
	}

	void Finalize() override
	{
		
	}
};