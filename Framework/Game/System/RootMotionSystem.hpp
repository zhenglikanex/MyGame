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

	void Update(fixed16 dt) override
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
						//registry.emplace_or_replace<Movement>(e, zero<vec3>(), it->velocity, it->angular_velocity);
						registry.emplace_or_replace<Movement>(e, zero<vec3>(), it->velocity, it->angular_velocity);
					}
				}
			}
		}
	}

	void Finalize() override
	{
	}
};