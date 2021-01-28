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
		auto view = registry.view<Animation,AnimationClip,Transform>();
		for (auto e : view)
		{
			auto& animtion = view.get<Animation>(e);
			auto& animtion_clip = view.get<AnimationClip>(e);
			auto& transform = view.get<Transform>(e);

			if (animtion.value)
			{
				auto iter = animtion.value->find(animtion_clip.name);
				if (iter != animtion.value->end())
				{
					if (auto it = iter->second.root_motions.find(animtion_clip.time); it != iter->second.root_motions.end())
					{
						registry.replace<Transform>(e, transform.position + it->second.delta_position, transform.forward);
						//todo:
						//registry.emplace_or_replace<Movement>();
					}
				}
			}
		}
	}

	void Finalize() override
	{
	}
};