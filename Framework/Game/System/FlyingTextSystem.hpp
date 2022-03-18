#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/UIService.hpp"

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

struct FlyingTextSystem : public System
{
	FlyingTextSystem(Registry& _registry)
		: System(_registry)
	{

	}

	~FlyingTextSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(float dt) override
	{
		auto& ui_service = registry.ctx<Locator>().Ref<UIService>();
		auto view = registry.view<ModifierAttribute>();
		for (auto e : view)
		{
			auto& modifier = view.get<ModifierAttribute>(e);
			if (modifier.attribute == GameplayAttributeType::Hp)
			{
				entt::entity target = registry.get(modifier.target);
				if (target == entt::null || !registry.all_of<Transform>(target))
				{
					continue;
				}
				auto& transform = registry.get<Transform>(target);
				ui_service.HpFlyingText(transform.position.x, transform.position.y, transform.position.z, modifier.value);
			}
		}
	}

	void Finalize() override
	{

	}
};