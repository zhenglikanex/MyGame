#pragma once

#pragma once

#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/RemoveGameplayEffectList.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/System.hpp"

struct RemoveGameplayEffectSystem : public System
{
	entt::observer remove_list{ registry, entt::collector.group<GameplayEffectList,RemoveGameplayEffectList>().update<RemoveGameplayEffectList>().where<GameplayEffectList>() };

	RemoveGameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~RemoveGameplayEffectSystem() {}

	bool Initialize() override
	{
		return true;
	}
	
	void FixedUpdate(fixed16 dt) override
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		if (remove_list.size() > 0)
		{
			for()
		}
	}

	void Finalize() override
	{

	}
};