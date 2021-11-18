#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>
#include <iostream>
#include "Framework/Game/GAS/GameplayEffect.hpp"

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package,sol::lib::string,sol::lib::math,sol::lib::table,sol::lib::debug,sol::lib::os);

	RegisterGameplayTag(lua);
	RegisterGameplayEffect(lua);

	lua.script_file("Script/main.lua", sol::script_default_on_error);
	
	std::function<void(uint32_t)> f = lua["f3"];

	f(10);

	GameplayEffect ge;
	
	lua["f"](ge);
	lua["f2"](ge.period.periodic_inhibition_policy);
	uint8_t v = static_cast<uint8_t>(ge.period.periodic_inhibition_policy);
	std::cout << v << std::endl;
}