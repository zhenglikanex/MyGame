#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayAbilityManager.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct TryGameplayAbilitySystem : public System
{

	TryGameplayAbilitySystem(entt::registry& _registry) : System(_registry) { }
	~TryGameplayAbilitySystem() {}

	bool Initialize() override
	{
		return true;
	}

	bool CheckCoolDown(const GameplayAbility& ga)
	{
		return true;
	}

	bool CheckCost(const GameplayAbility& ga)
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		GameplayAbilityManager& ga_manager = registry.ctx<GameplayAbilityManager>();

		if (auto try_ga = registry.try_ctx<TryGameplayAbility>())
		{
			for (auto& ga_list : try_ga->value)
			{
				for (auto& info : ga_list.second)
				{
					const GameplayAbility* ga = ga_manager.LoadGameplayAbility(info.ability);
					
					auto source_tags = registry.try_get<GameplayTags>(info.source);
					auto target_tags = registry.try_get<GameplayTags>(info.target);


					if (!CheckCoolDown(*ga))
					{
						continue;
					}

					if (!CheckCost(*ga))
					{
						continue;
					}

					// todo:其他检查
					sol::table script = ga->script["new"]();

					auto e = registry.create();
					auto& spec = registry.emplace<GameplayAbilitySpec>(e, ga, script, 1, info.source, info.target);
					
					script["Init"](script, e, spec);

					//todo:调用ga的lua逻辑
					ga->script["ActiveAbility"](ga->script);
				}

				ga_list.second.clear();
			}
		}
	}

	void Finalize() override
	{

	}
};
