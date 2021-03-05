#pragma once

#include "Framework/Game/Component/Health.hpp"

#include "Framework/Game/System.hpp"

struct ModifyHealthSystem : public System
{
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
			const auto& modify_health_list = view.get<ModifyHealthList>(e);

			// todo可以放到脚本中计算
			for (auto& modify_health : modify_health_list.value)
			{
				
			}
		}
	}

	void Finalize() override
	{

	}
};