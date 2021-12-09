#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"

#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/ExecuteGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/TryGameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAttributeSystem.hpp"

#include "Framework/Game/System/DelayTaskSystem.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/LuaBridge.hpp"
#include "Framework/Game/RandomEngine.hpp"


int main() {
	entt::registry registry;

	registry.set<sol::state>();
	registry.set<GameplayEffectManager>(registry);
	registry.set<GameplayAbilityManager>(registry);
	registry.set<RandomEngine>((uint32_t)1000);

	auto& lua = registry.ctx<sol::state>();
	lua.open_libraries(sol::lib::base, sol::lib::package,sol::lib::string,sol::lib::math,sol::lib::table,sol::lib::debug,sol::lib::os);
	lua.script_file("Script/main.lua", sol::script_default_on_error);

	RegisterLua(registry, lua);

	ApplyGameplayEffectSystem system1(registry);
	ExecuteGameplayEffectSystem system2(registry);
	TryGameplayAbilitySystem system3(registry);
	GameplayAttributeSystem system4(registry);
	DelayTaskSystem system5(registry);
	
	auto source = registry.create();
	auto target = registry.create();

	
	//GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, "AttackGE", 1, target);
	
	while (true)
	{
		system1.FixedUpdate(fixed16(1));
		system2.FixedUpdate(fixed16(1));
		system3.FixedUpdate(fixed16(1));
		system4.FixedUpdate(fixed16(1));
		system5.FixedUpdate(fixed16(1));
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}