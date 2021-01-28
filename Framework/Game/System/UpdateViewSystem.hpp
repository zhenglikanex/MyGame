#pragma once

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"

struct UpdateViewSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Transform>().update<Transform>().where<View>() };
	entt::observer animator{ registry,entt::collector.group<AnimationClip>().update<AnimationClip>().where<View>() };

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

			view.value->MovePosition(transform.position);
			view.value->MoveForward(transform.forward);
		}

		mover.clear();
	}

	void UpdateAnimation()
	{
		auto entt_view = registry.view<View,AnimationClip>();
		for (auto e : animator)
		{
			auto& view = entt_view.get<View>(e);
			const auto& clip = entt_view.get<AnimationClip>(e);

			view.value->PlayAnimation(clip.name);
		}

		animator.clear();
	}

	void Finalize() override
	{
	}
};