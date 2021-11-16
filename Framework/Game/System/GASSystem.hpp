#pragma once

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct GASSystem : public System
{
	entt::observer apply_ge_list{ registry,entt::collector.group<GameplayEffectList,GameplayTags,ApplyGameplayEffectList>()
		.update<ApplyGameplayEffectList>().where<GameplayEffectList>() };

	sol::function apply_gameplay_effect_func;

	GASSystem(entt::registry& _registry) : System(_registry) { }
	~GASSystem() {}

	bool Initialize() override
	{
		sol::state& lua = registry.ctx<sol::state>();
		apply_gameplay_effect_func = lua["apply_gameplay_effect"];
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		
	}

	void ApplyGameplayEffect()
	{
		
		auto view = registry.view<ApplyGameplayEffectList,GameplayEffectList, GameplayTags>();
		if (apply_ge_list.size() > 0)
		{
			for (auto e : apply_ge_list)
			{
				auto&[apply_ge_list,ge_list, tags] = view.get<ApplyGameplayEffectList,GameplayEffectList, GameplayTags>(e);
				apply_gameplay_effect_func(apply_ge_list, ge_list, tags);
			}
		}
	}

	void Finalize() override
	{

	}
};
