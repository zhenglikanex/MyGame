#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/Animation.hpp"

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
		for (auto e : registry.view<View,Animation>())
		{
			auto& animation = registry.get<Animation>(e);
			if (!animation.is_done)
			{
				animation.time += dt;
				animation.is_done = animation.time / GameConfig::kFrameTime >= fixed16(animation.max_frame);
			}
		}
	}

	void Finalize() override
	{

	}
};
