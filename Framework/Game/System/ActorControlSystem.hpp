#pragma once

#include <array>
#include <memory>
#include <algorithm>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/Fmt.hpp"

#include "Framework/Game/GAS/GameplayTagDefine.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"
#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"


inline vec2 SquareToCircle(const vec2& input)
{
	vec2 output = zero<vec2>();
	
	output.x = input.x * sqrt(1.f - (input.y * input.y) / 2.0f);
	output.y = input.y * sqrt(1.f - (input.x * input.x) / 2.0f);

	return output;
}

struct ActorControlSystem : public System
{
	ActorControlSystem(Registry& _registry)
		: System(_registry)
	{ }
	~ActorControlSystem() {}

	bool Initialize() override
	{
		return true;
	}

	bool CanMove(const GameplayTagCountContainer& tag_count)
	{
		return !tag_count.HasTagExact(GameplayTagDefine::kStateBlockMove);
	}
	
	bool CanTransIdle(const GameplayTagCountContainer& tag_count)
	{
		return !tag_count.HasTag(GameplayTagDefine::kState);
	}

	bool IsJump(const GameplayTagCountContainer& tag_count)
	{
		return tag_count.HasTag(GameplayTagDefine::kStateJump);
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<GameplayTagCountContainer, AnimationClip, CharacterMovement, Command>();
		for (auto e : view)
		{
			auto& [tag_count, animation_clip, character_movment,command] = view.get<GameplayTagCountContainer, AnimationClip,CharacterMovement,Command>(e);
			
			//自动转到空闲状态
			if (animation_clip.is_done && animation_clip.name != GameConfig::ActionAnimation::kIdle)
			{
				registry.replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
			}

			// 更新接地状态
			if (character_movment.grounding_status.is_stable_on_ground)
			{
				tag_count.SetTagCount(GameplayTagDefine::kStateGrounded, 1);
			}
			else
			{
				tag_count.SetTagCount(GameplayTagDefine::kStateGrounded, 0);
			}

			if ((command.x_axis != 0.f || command.y_axis != 0.f) && CanMove(tag_count))
			{
				vec2 input = SquareToCircle(vec2(command.x_axis, command.y_axis));

				//旋转
				auto forward = glm::normalize(vec3(input.x, 0.f, input.y));
				registry.emplace_or_replace<MovementForward>(e, forward);

				// 移动
				// 移动动画通过动画混合得到,这类动画数据的时候将混合参数作为key导出所有离散的动画数据
				if (tag_count.HasTagExact(GameplayTagDefine::kStateSprint))
				{
					input *= 2;
				}

				std::string name = fmt::format("{}|{:.1f}", GameConfig::ActionAnimation::kLocomotion, glm::length(input));
				if (name != animation_clip.name)
				{
					auto animation_clip = registry.try_get<AnimationClip>(e);
					if (animation_clip && animation_clip->name.find(GameConfig::ActionAnimation::kLocomotion) != std::string::npos)
					{
						registry.replace<AnimationClip>(e, name, animation_clip->time);
					}
					else
					{
						registry.emplace_or_replace<AnimationClip>(e, name);
					}
				}
			}

			if (IsJump(tag_count))
			{
				if (!registry.all_of<IgnoreGravity>(e))
				{
					registry.emplace<IgnoreGravity>(e);
				}
			}
			else
			{
				registry.remove<IgnoreGravity>(e);
			}

			for (auto& item : command.abilities)
			{
				if (item.second == AbilityStatus::kDown)
				{
					GameplayAbilityUtility::TryGameplayAbility(registry, e, item.first);
				}
			}
		}
	}

	void LateUpdate(float dt) override
	{
		
	}

	void Finalize() override
	{
		
	}
};