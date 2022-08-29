#pragma once

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/GAS/GameplayTagDefine.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

struct ActorSystem : public System
{
	explicit ActorSystem(Registry& _registry)
		: System(_registry)
	{ }

	bool Initialize() override
	{
		return true;
	}

	bool IsJump(const GameplayTagCountContainer& tag_count) const 
	{
		return tag_count.HasTag(GameplayTagDefine::kStateJump);
	}

	bool IsHurt(const GameplayTagCountContainer& tag_count) const
	{
		return tag_count.HasTagExact(GameplayTagDefine::kStateHurtNormal);
	}

	bool IsFall(const AnimationClip& animation_clip, const GameplayTagCountContainer& tag_count, entt::entity e) const
	{
		return !tag_count.HasTag(GameplayTagDefine::kStateGrounded) && !IsJump(tag_count)
			&& (animation_clip.is_done || animation_clip.name == GameConfig::ActionAnimation::kIdle)
			&& animation_clip.name != GameConfig::ActionAnimation::kFall;
	}

	bool IsIdle(const AnimationClip& animation_clip, const GameplayTagCountContainer& tag_count, entt::entity e) const
	{
		return tag_count.HasTag(GameplayTagDefine::kStateGrounded)
			&& (animation_clip.is_done || animation_clip.name == GameConfig::ActionAnimation::kFall)
			&& animation_clip.name != GameConfig::ActionAnimation::kIdle;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Local,GameplayTagCountContainer, AnimationClip, CharacterMovement>();
		for (auto e : view)
		{
			auto&[tag_count, animation_clip, character_movement] = view.get<GameplayTagCountContainer, AnimationClip, CharacterMovement>(e);

			UpdateGrounded(character_movement, tag_count);
			UpdateGravity(tag_count, e);
			UpdateAnimation(animation_clip, tag_count,e);
		}
	}

	void UpdateAnimation(const AnimationClip& animation_clip,const GameplayTagCountContainer& tag_count,entt::entity e)
	{
		if (IsHurt(tag_count))
		{
			if (animation_clip.name != GameConfig::ActionAnimation::kHurt)
			{
				registry.replace<AnimationClip>(e, GameConfig::ActionAnimation::kHurt);
			}
			else if (animation_clip.is_done)
			{
				registry.patch<GameplayTagCountContainer>(e, [](GameplayTagCountContainer& tag_count) {
					tag_count.SetTagCount(GameplayTagDefine::kStateHurtNormal, 0);
					});
			}
		}

		if(IsFall(animation_clip,tag_count,e))
		{
			registry.replace<AnimationClip>(e, GameConfig::ActionAnimation::kFall);
		}

		if (IsIdle(animation_clip,tag_count,e))
		{
			registry.replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}
	}

	void UpdateGrounded(const CharacterMovement& character_movement,GameplayTagCountContainer& tag_count)
	{
		if (character_movement.grounding_status.is_stable_on_ground)
		{
			tag_count.SetTagCount(GameplayTagDefine::kStateGrounded, 1);
		}
		else
		{
			tag_count.SetTagCount(GameplayTagDefine::kStateGrounded, 0);
		}
	}

	void UpdateGravity(const GameplayTagCountContainer& tag_count,entt::entity e)
	{
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
	}

	void Finalize() override
	{
		
	}
};