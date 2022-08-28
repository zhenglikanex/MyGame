#pragma once

#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

struct RootMotionSystem : public System
{
	explicit RootMotionSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Animation,AnimationClip>();
		for (auto e : view)
		{
			auto& animation = view.get<Animation>(e);
			auto& animation_clip = view.get<AnimationClip>(e);

			if(animation_clip.name == "Jump")
			{
				int a = 10;
				INFO("23");
			}

			if (animation.value)
			{
				auto iter = animation.value->clips.find(animation_clip.name);
				if (iter != animation.value->clips.end())
				{
					size_t frame = std::min(static_cast<size_t>(animation_clip.time / GameConfig::kFrameTime), iter->second.root_motions.size() - 1);

					auto& root_motion = iter->second.root_motions[frame];
					if (!registry.all_of<CharacterMovement>(e))
					{
						registry.emplace_or_replace<Movement>(e, root_motion.velocity, root_motion.angular_velocity);
					}
					else
					{
						auto& forward = registry.get<MovementForward>(e);
						auto& character_movement = registry.get<CharacterMovement>(e);
						character_movement.base_velocity = forward.value * root_motion.velocity.z;
						character_movement.base_velocity.y += root_motion.velocity.y;
					}
				}
			}
		}
	}

	void Finalize() override
	{
	}
};