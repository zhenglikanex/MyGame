#define CATCH_CONFIG_MAIN

#include "3rdparty/include/catch2/catch.hpp"
#include <chrono>
#include "Framework/Proto/NetMessage.hpp"
#include "Framework/Proto/Battle.pb.h"
#include "Framework/Game/Component/TransformState.hpp"
#include <any>
#include <iostream>
#include "Framework/Game/Math.hpp"

#include "Framework/Game/GAS/GameplayTag.hpp"

using namespace std::chrono;

struct Serialize1
{
	
};


TEST_CASE_METHOD(Serialize1, "NetMessage")
{
	std::any a = 10;
	std::vector<GameplayTag> vec;
	std::any b = vec;
	std::string s("nibi");
	std::any c = s;
	

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
