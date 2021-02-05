#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"


struct AnimationSystem : public System
{
	AnimationSystem(entt::registry& _registry) : System(_registry) { }
	~AnimationSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<Animation, AnimationClip>();
		for (auto e : view)
		{
			auto& animation = view.get<Animation>(e);
			if (animation.value)
			{
				auto& animation_clip = view.get<AnimationClip>(e);

				// loop play
				if (animation_clip.is_done)
				{
					animation_clip.is_done = false;
					animation_clip.time = fixed16(0);
				}

				if (!animation_clip.is_done)
				{
					animation_clip.time += dt;
				}

				auto iter = animation.value->clips.find(animation_clip.name);
				if (iter != animation.value->clips.cend())
				{
					if (animation_clip.time >= iter->second.length)
					{
						animation_clip.is_done = true;
					}
				}
			}
		}
	}

	void Finalize() override
	{

	}
};
