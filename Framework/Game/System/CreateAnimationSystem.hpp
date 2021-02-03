#pragma once

#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/AnimationConfig.hpp"

struct CreateAnimationSystem : System
{
	CreateAnimationSystem(entt::registry& registry) 
		: System(registry)
	{

	}

	~CreateAnimationSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt)
	{
		auto view = registry.view<AnimationAsset>(entt::exclude_t<Animation>{});
		for (auto e : view)
		{
			auto& asset = view.get<AnimationAsset>(e);
			
			const auto& animation_config = registry.ctx<AnimationConfig>();
			auto& animation_info = animation_config.GetEntry(asset.value);
			registry.emplace<Animation>(e, &animation_info);
		}
	}

	void Finalize()
	{
		
	}
};