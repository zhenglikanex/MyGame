#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/GAS/GameplayTagDefine.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/MoveCommand.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

inline vec2 SquareToCircle(const vec2& input)
{
	vec2 output = zero<vec2>();

	output.x = input.x * sqrt(1.f - (input.y * input.y) / 2.0f);
	output.y = input.y * sqrt(1.f - (input.x * input.x) / 2.0f);

	return output;
}

struct MoveCommandSystem : public System
{
	explicit MoveCommandSystem(Registry& _registry)
		: System(_registry)
	{ }

	bool Initialize() override
	{
		return true;
	}

	bool CanMove(const GameplayTagCountContainer& tag_count) const
	{
		return !tag_count.HasTagExact(GameplayTagDefine::kStateBlockMove) && tag_count.HasTagExact(GameplayTagDefine::kStateGrounded);
	}

	bool IsSprint(const GameplayTagCountContainer& tag_count) const
	{
		return tag_count.HasTagExact(GameplayTagDefine::kStateSprint);
	}

	void FixedUpdate(float dt) override
	{
		auto& move_command = registry.ctx_or_set<MoveCommand>();
		for (auto& command : move_command.value)
		{
			entt::entity target = registry.get(command.target);
			if (target != entt::null && registry.all_of<GameplayTagCountContainer>(target))
			{
				auto& tag_count = registry.get<GameplayTagCountContainer>(target);
				if (!CanMove(tag_count))
				{
					continue;
				}

				vec2 input = SquareToCircle(vec2(command.x, command.y));

				Turn(input, target);
				PlayAnimation(tag_count, input, target);
			}

			move_command.value.clear();
		}
	}

	void Turn(const vec2& input, entt::entity target)
	{
		auto forward = glm::normalize(vec3(input.x, 0.f, input.y));
		registry.emplace_or_replace<MovementForward>(target, forward);
	}

	void PlayAnimation(const GameplayTagCountContainer& tag_count, vec2& input, entt::entity target)
	{
		if (IsSprint(tag_count))
		{
			input *= 2;
		}

		// 移动
		// 移动动画通过动画混合得到,这类动画数据的时候将混合参数作为key导出所有离散的动画数据
		std::string name = fmt::format("{}|{:.1f}", GameConfig::ActionAnimation::kLocomotion, glm::length(input));
		auto animation_clip = registry.try_get<AnimationClip>(target);
		if (animation_clip)
		{
			// 更好的适配Unity的动画融合
			if (animation_clip && animation_clip->name.find(GameConfig::ActionAnimation::kLocomotion) != std::string::npos)
			{
				registry.replace<AnimationClip>(target, name, animation_clip->time);
			}
			else
			{
				registry.replace<AnimationClip>(target, name);
			}
		}
		else
		{
			registry.emplace<AnimationClip>(target, name);
		}
	}

	void Finalize() override
	{

	}
};