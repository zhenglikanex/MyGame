#pragma once

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityTimelineTask.hpp"

#include "Framework/Game/System.hpp"

struct UpdateViewSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Transform, View>().update<Transform>().where<View>() };
	entt::observer animator{ registry, entt::collector.group<AnimationClip, View>().update<AnimationClip>().where<View>() };
	entt::observer timelines{ registry, entt::collector.group<GameplayAbilityTimelineTask, View>().update<GameplayAbilityTimelineTask>().where<View>() };

	explicit UpdateViewSystem(Registry& _registry) 
		: System(_registry)
	{

	}

	bool Initialize() override
	{
		return true;
	}


	void Update(float dt) override
	{
		UpdateTransform();
		UpdateAnimation();

		mover.clear();
		animator.clear();
		timelines.clear();
	}

	void UpdateTransform()
	{
		auto entt_view = registry.view<View, Transform>();
		for (auto e : mover)
		{
			auto& view = entt_view.get<View>(e);
			const auto& transform = entt_view.get<Transform>(e);

			view.value->UpdatePosition(transform.position);
			view.value->UpdateRotation(transform.rotation);
		}
	}

	void UpdateAnimation()
	{
		auto entt_view = registry.view<View,Animation,AnimationClip>();
		for (auto e : animator)
		{
			auto& view = entt_view.get<View>(e);
			auto& animation = entt_view.get<Animation>(e);
			const auto& clip = entt_view.get<AnimationClip>(e);
			
			if (clip.time == 0.0f)
			{
				view.value->PlayAnimation(clip.name);
			}
			else
			{
				auto iter = animation.value->clips.find(clip.name);
				if (iter != animation.value->clips.end())
				{
					auto time = clip.time / iter->second.length;
					view.value->PlayAnimation(clip.name,time,clip.is_timeline);
				}
				else
				{
					view.value->PlayAnimation(clip.name, clip.is_timeline);
				}
			}
		}
	}

	void Finalize() override
	{

	}
};