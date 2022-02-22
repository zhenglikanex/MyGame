#pragma once

#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

struct RootMotionSystem : public System
{
	RootMotionSystem(Registry& _registry) : System(_registry) { }
	~RootMotionSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Animation,AnimationClip>();
		for (auto e : view)
		{
			auto& animtion = view.get<Animation>(e);
			auto& animation_clip = view.get<AnimationClip>(e);

			if (animtion.value)
			{
				auto iter = animtion.value->clips.find(animation_clip.name);
				if (iter != animtion.value->clips.end())
				{
					uint32_t frame = animation_clip.time / GameConfig::kFrameTime;

					if (frame < iter->second.root_motions.size())
					{
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
					else
					{
						if (!registry.all_of<CharacterMovement>(e))
						{
							registry.remove<Movement>(e);
						}
						else
						{
							auto& character_movement = registry.get<CharacterMovement>(e);
							character_movement.base_velocity = vec3(0);
						}
					}
				}
			}
		}
	}

	void Finalize() override
	{
	}
};