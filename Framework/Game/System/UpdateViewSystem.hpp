#pragma once

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

struct UpdateViewSystem : public System
{
	entt::observer mover{ registry, entt::collector.group<Transform>().update<Transform>().where<View>() };
	entt::observer animator{ registry,entt::collector.group<Animation>().update<Animation>().where<View>() };

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
		for (auto e : mover)
		{
			auto& view = registry.get<View>(e);
			const auto& transform = registry.get<Transform>(e);

			view.value->MovePosition(transform.position);
			view.value->MoveForward(transform.forward);
		}

		mover.clear();
	}

	void UpdateAnimation()
	{
		for (auto e : animator)
		{
			auto& view = registry.get<View>(e);
			const auto& animation = registry.get<Animation>(e);

			view.value->PlayAnimation(animation.name);
		}

		animator.clear();
	}

	void Finalize() override
	{
	}
};