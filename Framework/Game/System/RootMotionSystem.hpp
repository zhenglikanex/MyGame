#pragma once

#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"

#include "Framework/Game/System.hpp"

struct RootMotionSystem : public System
{
	RootMotionSystem(entt::registry& _registry) : System(_registry) { }
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
			auto& animtion_clip = view.get<AnimationClip>(e);

			if (animtion.value)
			{
				auto iter = animtion.value->clips.find(animtion_clip.name);
				if (iter != animtion.value->clips.end())
				{
					auto it = std::find_if(iter->second.root_motions.crbegin(), iter->second.root_motions.crend(), [&animtion_clip](const RootMotion& root_motion)
					{
						return animtion_clip.time >= root_motion.time;
					});

					if (it != iter->second.root_motions.crend())
					{
						if (!registry.all_of<CharacterMovement>(e))
						{
							auto movement = registry.try_get<Movement>(e);
							if (movement)
							{
								movement->velocity = it->velocity;
								movement->angular_velocity = it->angular_velocity;
							}
							else
							{
								registry.emplace<Movement>(e,it->velocity, it->angular_velocity);
							}
						}
						else
						{
							auto& forward = registry.get<MovementForward>(e);
							auto& character_movement = registry.get<CharacterMovement>(e);
							character_movement.base_velocity = forward.value * it->velocity.z;
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