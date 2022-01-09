#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/Utility/AnimEventUtility.hpp"

#include "Framework/Game/System.hpp"


struct AnimationSystem : public System
{
	AnimationSystem(entt::registry& _registry) : System(_registry) { }
	~AnimationSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<Local,Animation,AnimationClip>();
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

				uint32_t frame = static_cast<float>(animation_clip.time) / static_cast<float>(GameConfig::kFrameTime);
				auto it = iter->second.anim_events_map.find(frame);
				if (it != iter->second.anim_events_map.end())
				{
					for (auto& event : it->second)
					{
						AnimEventUtility::SendAnimEvent(registry, e, event.name);
					}
				}
			}
		}
	}

	void Finalize() override
	{

	}
};
