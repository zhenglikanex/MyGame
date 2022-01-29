
#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"

#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/TryGameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEventSystem.hpp"
#include "Framework/Game/System/DestorySystem.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/LuaBridge.hpp"
#include "Framework/Game/RandomEngine.hpp"

#include "3rdparty/include/catch2/catch.hpp"

//struct Test 
//{
//
//};
//
//TEST_CASE_METHOD(Test, "TAG")
//{
//	GameplayTag tag1("Test.Tag1");
//	GameplayTag tag2("Test.Tag2");
//	GameplayTag tag3("Test.Tag3");
//	GameplayTag tag4("Test.Tag1");
//	GameplayTag tag5("Test");
//
//	REQUIRE(tag1 == tag4);
//	REQUIRE(tag1.Contain(tag4));
//	REQUIRE(!tag1.Contain(tag2));
//	REQUIRE(tag5.Contain(tag1));
//	REQUIRE(tag5.Contain(tag2));
//	REQUIRE(tag5.Contain(tag3));
//	REQUIRE(tag5.Contain(tag4));
//
//	GameplayTagArray array1;
//	REQUIRE(array1.Empty());
//	
//	array1.Push(tag1);
//	REQUIRE(array1.Find(tag1));
//	REQUIRE(!array1.Find(tag2));
//	REQUIRE(array1.Find(tag4));
//
//	array1.Clear();
//	REQUIRE(array1.Empty());
//	array1.Push(tag5);
//	array1.Push(tag2);
//	REQUIRE(!array1.Find(tag1));
//
//
//	GameplayTagArray array2;
//	array2.Push(tag1);
//
//	REQUIRE(!array1.HasAny(array2));
//	REQUIRE(array2.HasAny(array1));
//
//	REQUIRE(!array2.HasAll(array1));
//	REQUIRE(array1.HasAll(array2));
//	array2.Push(GameplayTag("Test2"));
//	REQUIRE(!array1.HasAll(array2));
//}
//
//TEST_CASE_METHOD(Test, "GAS")
//{
//	
//}

int main()
{
	/*
	entt::registry registry;

	registry.set<sol::state>();
	registry.set<GameplayEffectManager>(registry);
	registry.set<GameplayAbilityManager>(registry);
	registry.set<RandomEngine>((uint32_t)1000);

	auto& lua = registry.ctx<sol::state>();
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::os);
	lua.script_file("Script/main.lua", sol::script_default_on_error);

	sol::function::set_default_handler(lua["__G__TRACKBACK__"]);

	//RegisterLua(registry, lua);

	ApplyGameplayEffectSystem system1(registry);
	TryGameplayAbilitySystem system3(registry);
	GameplayAttributeSystem system4(registry);
	DelayTaskSystem system5(registry);
	GameplayEventSystem system6(registry);
	DestorySystem system7(registry);

	auto source = registry.create();
	auto target = registry.create();

	//GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, "AttackGE", 1, target);
	GameplayAbilityUtility::TryGameplayAbility(registry, source, "AttackGA", target);

	auto& delay_task = registry.ctx_or_set<DelayTask>();
	delay_task.entity_by_events[source].emplace_back(1, [&registry,target]()
	{
		GameplayEventUtility::SendGameplayEvent(registry, GameplayTag("GA.AttackGA"),target);
	});

	registry.emplace<GameplayAttributeSet>(source);
	registry.emplace<ModifierAttributeSet>(source);
	registry.emplace<ModifierBaseAttributeSet>(source);
	registry.emplace<GameplayAttributeSet>(target);
	registry.emplace<ModifierAttributeSet>(target);
	registry.emplace<ModifierBaseAttributeSet>(target);

	GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, "IngoreTestGE", 1, target);
	GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, "HeroBaseGE", 1, source);
	GameplayEffectUtility::ApplyGameEffectToTarget(registry, target, "HeroBaseGE", 1, target);
	
	while (true)
	{
		system3.FixedUpdate(fixed16(1));
		system5.FixedUpdate(fixed16(1));
		system6.FixedUpdate(fixed16(1));
		system1.FixedUpdate(fixed16(1));
		system4.FixedUpdate(fixed16(1));
		system7.FixedUpdate(fixed16(1));

		auto view = registry.view<GameplayAttributeSet>();
		for (auto e : view)
		{
			auto& attribute_set = view.get<GameplayAttributeSet>(e);
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	*/
	return 0;
}