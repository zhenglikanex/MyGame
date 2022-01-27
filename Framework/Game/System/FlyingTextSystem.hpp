#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/UIService.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

struct FlyingTextSystem : public ObserverSystem
{
	entt::observer* updatelist;

	FlyingTextSystem(entt::registry& _registry)
		: ObserverSystem(_registry)
		, updatelist(nullptr)
	{

	}

	~FlyingTextSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
		auto& ui_service = registry.ctx<Locator>().Ref<UIService>();
		auto view = registry.view<ModifierAttribute>();
		for (auto e : view)
		{
			auto& modifier = view.get<ModifierAttribute>(e);
			
			auto&it = unique_map.value.find(modifier.target);
			if (it == unique_map.value.end())
			{
				continue;
			}

			entt::entity e = it->second;
			if (!registry.valid(e) || !registry.all_of<Transform>(e))
			{
				continue;
			}

			if (modifier.attribute == GameplayAttributeType::Hp)
			{
				auto& transform = registry.get<Transform>(e);
				ui_service.HpFlyingText(transform.position.x,transform.position.y,transform.position.z,modifier.value);
			}
		}
	}

	void Finalize() override
	{

	}
};