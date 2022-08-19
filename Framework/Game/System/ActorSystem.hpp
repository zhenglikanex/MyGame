#pragma once

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/GAS/GameplayTagDefine.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

#include "Framework/Game/Component/AnimationClip.hpp"

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

	bool IsJump(const GameplayTagCountContainer& tag_count)
	{
		return tag_count.HasTag(GameplayTagDefine::kStateJump);
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Local,GameplayTagCountContainer, AnimationClip, CharacterMovement>();
		for (auto e : view)
		{
			auto&[tag_count, animation_clip, character_movement] = view.get<GameplayTagCountContainer, AnimationClip, CharacterMovement>(e);
			
			ProcessIdle(animation_clip, e);
			UpdateGrounded(character_movement, tag_count);
			UpdateGravity(tag_count, e);
		}
	}

	void ProcessIdle(const AnimationClip& animation_clip, entt::entity e)
	{
		if (animation_clip.is_done && animation_clip.name != GameConfig::ActionAnimation::kIdle)
		{
			registry.replace<AnimationClip>(e, GameConfig::ActionAnimation::kIdle);
		}
	}

	void UpdateGrounded(const CharacterMovement& character_movement,GameplayTagCountContainer& tag_count)
	{
		auto& game_state = registry.ctx<GameState>();
		INFO("frame{} is_stable_on_ground {}", game_state.run_frame, character_movement.grounding_status.is_stable_on_ground);

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