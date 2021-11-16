#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"


int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	RegisterApplyGameplayEffectList(lua);
	RegisterGameplayEffectList(lua);
	lua.script_file("Script/main.lua", sol::script_default_on_error);
	ApplyGameplayEffectList list;
	list.value = { "num","a","b" };
	lua["f"](list);

	GameplayEffectList ges;
	lua["f2"](ges);

	auto result = lua["f3"](ges);
}