#include "3rdparty/include/sol/sol.hpp"
#include <fstream>
#include <cassert>

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file("Script/main.lua", sol::script_pass_on_error);
	
	
}