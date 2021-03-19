#pragma once

#include "Framework/Game/Component/Health.hpp"

#include "Framework/Game/System.hpp"

struct ModifyHealthSystem : public System
{
	entt::observer modify_list{ registry, entt::collector.group<Health,ModifyHealthList>().update<ModifyHealthList>() };

	ModifyHealthSystem(entt::registry& _registry) : System(_registry) { }
	~ModifyHealthSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<Health, ModifyHealthList>();
		for (auto e : view)
		{
			auto& health = view.get<Health>(e);
			auto& modify_health_list = view.get<ModifyHealthList>(e);

			// todo可以放到脚本中计算
			for (auto& modify_health : modify_health_list.value)
			{
				
			}

			modify_health_list.value.clear();
		}
	}

	void Finalize() override
	{

	}
};