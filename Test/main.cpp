#pragma once

#include "entt/entt.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

struct Test
{
	int a;
};

int main()
{	
	entt::registry r;
	entt::observer o{ r,entt::collector.group<Test>().update<> };
	entt::observer o1{ r,entt::collector.update<Test>() };

	system("pause");
	return 0;
}