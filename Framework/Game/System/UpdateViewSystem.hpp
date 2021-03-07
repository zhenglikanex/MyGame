#pragma once

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"

struct UpdateViewSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Transform,View>().update<Transform>() };
	entt::observer animator{ registry,entt::collector.group<AnimationClip,View>().update<AnimationClip>() };

	UpdateViewSystem(entt::registry& _registry) : System(_registry) { }
	~UpdateViewSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		UpdateTransform();
		UpdateAnimation();
	}

	void UpdateTransform()
	{
		auto entt_view = registry.view<View, Transform>();
		for (auto e : mover)
		{
			auto& view = entt_view.get<View>(e);
			const auto& transform = entt_view.get<Transform>(e);

			view.value->UpdatePosition(transform.position);
			view.value->UpdateForward(transform.forward);
			//view.value->MoveForward(transform.forward);
		}

		mover.clear();
	}

	void UpdateAnimation()
	{
		auto entt_view = registry.view<View,Animation,AnimationClip>();
		for (auto e : animator)
		{
			auto& view = entt_view.get<View>(e);
			auto& animation = entt_view.get<Animation>(e);
			const auto& clip = entt_view.get<AnimationClip>(e);
			

			view.value->PlayAnimation(clip.name);
			/*if (clip.time == fixed16(0.0f))
			{
				
			}
			else
			{
				auto iter = animation.value->clips.find(clip.name);
				if (iter != animation.value->clips.end())
				{
					auto time = clip.time / iter->second.length;
					view.value->PlayAnimation(clip.name, static_cast<float>(time));
				}
			}*/
		}

		animator.clear();
	}

	void Finalize() override
	{
	}
};