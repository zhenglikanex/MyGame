#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package,sol::lib::string,sol::lib::math,sol::lib::table,sol::lib::debug,sol::lib::os);
	RegisterApplyGameplayEffectList(lua);
	RegisterGameplayEffectList(lua);
	RegisterGameplayTags(lua);
	
	lua.script_file("Script/main.lua", sol::script_default_on_error);

	GameplayTags tags;
	lua["f4"](tags);
	
}