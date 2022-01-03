#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/LocalId.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAbilitySystem : public System
{
	GameplayAbilitySystem(entt::registry& _registry) : System(_registry) { }
	~GameplayAbilitySystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		ActiveAbility();
		UpdateAbility(static_cast<float>(dt));
	}

	void ActiveAbility()
	{
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
		auto view = registry.view<GameplayAbilitySpec>(entt::exclude<GameplayAbilityScript>);
		for (auto e : view)
		{
			auto& spec = view.get<GameplayAbilitySpec>(e);

			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);
			sol::table instance = ga.script["new"](e, spec);
			auto& script = registry.emplace<GameplayAbilityScript>(e,instance);

			if (!spec.activation)
			{
				spec.activation = true;
				script.value["ActiveAbility"](script.value);
			}
		}
	}

	void UpdateAbility(float dt)
	{

	}

	void Finalize() override
	{

	}
};
