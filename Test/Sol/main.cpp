#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>
#include <iostream>
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "3rdparty/include/entt/entt.hpp"

int main() {


	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package,sol::lib::string,sol::lib::math,sol::lib::table,sol::lib::debug,sol::lib::os);

	entt::registry registry;

	entt::entity e1 = registry.create();
	sol::table t = lua.safe_script(R"(
		
		local t = {}	
		t.val = 10
		function t:func()
			t.val = t.val + 30
			print(t.val)
		end

		return t
	)");
	registry.emplace<sol::table>(e1, t);

	entt::entity e2 = registry.create();
	sol::table t2 = lua.safe_script(R"(
		
		local t = {}	
		t.val = 10
		function t:func()
			t.val = t.val + 30
			print(t.val)
		end

		return t
	)");
	registry.emplace<sol::table>(e2, t2);

	registry.destroy(e1);

	auto table = registry.get<sol::table>(e2);
	table["func"](table);
}