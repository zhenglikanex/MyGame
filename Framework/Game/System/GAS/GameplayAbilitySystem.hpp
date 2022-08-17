#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAbilitySystem : public System
{
	GameplayAbilitySystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
		auto view = registry.view<Local, GameplayAbilitySpec>(entt::exclude<GameplayAbilityScript>);
		for (auto e : view)
		{
			auto& [local, spec] = view.get<Local, GameplayAbilitySpec>(e);

			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);
			if (ga.script)
			{
				sol::table instance = ga.script["new"](e);
				auto& script = registry.emplace<GameplayAbilityScript>(e, instance);

				if (!spec.activation)
				{
					spec.activation = true;
					script.value["ActiveAbility"](script.value);
				}
			} 
			else
			{
				registry.emplace<Destroy>(e);
				assert("LoadGameplayAbilityFailed!" && false);
			}
		}
	}

	void Finalize() override
	{

	}
};
