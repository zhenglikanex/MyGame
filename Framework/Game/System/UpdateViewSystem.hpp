#pragma once

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"

void func(const entt::registry& reg, entt::entity e)
{
	//auto move = reg.get<Movement>(e);
	//int a = 10;
}

struct UpdateViewSystem : public ObserverSystem
{
	

	entt::observer* mover;
	entt::observer* animator;

	UpdateViewSystem(entt::registry& _registry) 
		: ObserverSystem(_registry) 
		, mover(nullptr)
		, animator(nullptr)
	{

	}

	~UpdateViewSystem() {}

	bool Initialize() override
	{
		registry.on_update<Transform>().connect<&func>();

		mover = CreateObserver(entt::collector.group<Transform, View>().update<Transform>().where<View>());
		animator = CreateObserver(entt::collector.group<AnimationClip, View>().update<AnimationClip>().where<View>());

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
		for (auto e : *mover)
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
		for (auto e : *animator)
		{
			auto& view = entt_view.get<View>(e);
			auto& animation = entt_view.get<Animation>(e);
			const auto& clip = entt_view.get<AnimationClip>(e);
			
			if (clip.time == fixed16(0.0f))
			{
				view.value->PlayAnimation(clip.name);
			}
			else
			{
				auto iter = animation.value->clips.find(clip.name);
				if (iter != animation.value->clips.end())
				{
					auto time = clip.time / iter->second.length;
					view.value->PlayAnimation(clip.name, static_cast<float>(time));
				}
			}
		}
	}

	void Finalize() override
	{

	}

	// 注意因为观察对象包含view,所以在回滚的时候需要不需要清掉
	void Connect() override
	{
		mover->connect(registry,entt::collector.group<Transform, View>().update<Transform>().where<View>());
		animator->connect(registry,entt::collector.group<AnimationClip, View>().update<AnimationClip>().where<View>());
	}

	void Disconnect() override
	{
		mover->disconnect();
		animator->disconnect();
	}
};