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
	entt::observer o{ r,entt::collector.update<Test>() };
	entt::observer o1{ r,entt::collector.update<Test>() };

	auto e = r.create();
	r.emplace<Test>(e, 10);

	r.replace<Test>(e, 20);

	o.clear();
	std::cout << o.size() << std::endl;
	std::cout << o1.size() << std::endl;

	system("pause");
	return 0;
}