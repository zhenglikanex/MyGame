#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	RegisterApplyGameplayEffectList(lua);
	lua.script_file("Script/main.lua", sol::script_pass_on_error);
	ApplyGameplayEffectList list;
	list.value = { "num","a","b" };
	lua["f"](list);
}