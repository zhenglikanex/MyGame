#define CATCH_CONFIG_MAIN

#include "3rdparty/include/catch2/catch.hpp"
#include <chrono>
#include <any>
#include <iostream>
#include "Framework/Game/Math.hpp"
#include <variant>
#include <iostream>
#include <bitset>

using namespace std::chrono;

struct Serialize1
{
	
};


template<class Component>
int Func1(uint32_t i)
{
	std::cout << i << std::endl;
	return 1;
}

TEST_CASE_METHOD(Serialize1, "NetMessage")
{
	
	 Serialize1 S{};
	std::unordered_map<int, Serialize1> map;
	map.emplace(1, S);
	

	//std::numeric_limits<float>::epsilon()
	/*auto v2 = ProjectOnPlane(vec3(0,1.0,0), vec3(0, 1.0, 0));
	auto v1 = glm::normalize(vec3(0,0,0));
	auto v = LookRotation(vec3(1, 0, 0), vec3(0, 1, 0));

	int a = 10;
	int* p1 = &a;
	int* p2 = &a;
	int* p3 = p1;

	auto va = glm::cos(glm::radians(90.f));

	std::cout << (&p1 == &p2) << (p1 == p3) << (p2 == p3) << std::endl;*/
	/*std::vector<int> iv;
	std::unique_ptr<NetMessage> m = std::make_unique<NetMessage>();
	std::any a = std::make_any<std::unique_ptr<NetMessage>>(std::move(m));*/
	//std::shared_ptr<void> vm = std::move(m);
}
