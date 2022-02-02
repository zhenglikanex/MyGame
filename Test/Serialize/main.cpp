#define CATCH_CONFIG_MAIN

#include "3rdparty/include/catch2/catch.hpp"
#include <chrono>
#include "Kanex.hpp"
#include "3rdparty/include/glm/gtc/quaternion.hpp"
#include "Framework/Game/Math.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Command.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct Serialize
{
	kanex::Buffer buffer{ 10000000 };
	kanex::BinaryStream stream{ buffer };
	kanex::BinaryOutputArchive oar{ stream };
	kanex::BinaryInputArchive iar{ stream };
};

class Timer
{
public:
	Timer()
	{
		start_ = std::chrono::system_clock::now();
	}

	double Elapsed()
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now() - start_).count();
	}
private:
	std::chrono::system_clock::time_point start_;
};

class AutoTimer
{
public:
	AutoTimer() = default;
	AutoTimer(const std::string& _name) : name(_name) { }
	~AutoTimer() {
		std::cout << name << ":" << timer.Elapsed() * 1000 << " ms" << std::endl;
	}

	Timer timer;
	std::string name;
};

TEST_CASE_METHOD(Serialize, "ActorAsset")
{
	ActorAsset output("131231231");

	oar(output);

	ActorAsset input;

	iar(input);

	REQUIRE(output.value == input.value);
}

TEST_CASE_METHOD(Serialize, "Fixed")
{
	kanex::Buffer buffer(1000);
	kanex::BinaryStream stream(buffer);
	kanex::BinaryOutputArchive oar(stream);
	kanex::BinaryInputArchive iar(stream);

	fixed16 src1(100);
	fixed16 src2(200);
	fixed16 src3(0.3);

	std::vector<fixed16> vec;
	vec.push_back(src1);
	vec.push_back(src2);
	vec.push_back(src3);

	oar(src1);
	oar(src2);
	oar(src3);
	oar(vec);

	fixed16 out1;
	fixed16 out2;
	fixed16 out3;
	std::vector<fixed16> out_vec;

	iar(out1);
	iar(out2);
	iar(out3);
	iar(out_vec);

	REQUIRE(src1 == out1);
	REQUIRE(src2 == out2);
	REQUIRE(src3 == out3);
	REQUIRE(vec == out_vec);

	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Math")
{
	vec3 src1(0, 0, 0);
	vec3 src2(1.7, 8.9, 104);

	std::vector<vec3> vec;
	vec.push_back(src1);
	vec.push_back(src2);

	oar(src1);
	oar(src2);
	oar(vec);

	vec3 out1;
	vec3 out2;

	std::vector<vec3> out;

	iar(out1);
	iar(out2);
	iar(out);

	REQUIRE(src1 == out1);
	REQUIRE(src2 == out2);
	REQUIRE(vec == out);
	REQUIRE(buffer.IsFinish());

	quat src3 = glm::identity<quat>();
	oar(src3);
	
	quat out3;
	iar(out3);

	REQUIRE(src3 == out3);
	REQUIRE(buffer.IsFinish());

	mat4 src4 = glm::identity<mat4>();
	oar(src4);

	mat4 out4;
	iar(out4);

	REQUIRE(src4 == out4);
	

	vec4 src5(1.7, 8.9, 104,3.0);
	oar(src5);

	vec4 out5;
	iar(out5);

	REQUIRE(src5 == out5);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Movement")
{
	Movement src(vec3(10, 20, 30.3), vec3(30, 20, 20));

	oar(src);
	
	Movement out;

	iar(out);

	REQUIRE(src.velocity == out.velocity);
	REQUIRE(src.angular_velocity == out.angular_velocity);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Transform")
{
	/*std::vector<Transform> outvec;
	Transform output(vec3(0.222, 0.333, 0.1), quat(10, 20, 30, 40));
	outvec.push_back(output);
	outvec.push_back(output);
	outvec.push_back(output);
	oar(output);
	oar(outvec);

	std::vector<Transform> invec;
	Transform input;
	iar(input);
	iar(invec);

	REQUIRE(output.position == input.position);
	REQUIRE(output.rotation == output.rotation);
	REQUIRE(invec.size() == outvec.size());
	REQUIRE(buffer.IsFinish());*/
}

TEST_CASE_METHOD(Serialize, "Weapon")
{

}

TEST_CASE_METHOD(Serialize, "Attributes")
{

}

TEST_CASE_METHOD(Serialize, "Health")
{

}

TEST_CASE_METHOD(Serialize, "Set")
{
	std::set<int> output1;
	output1.emplace(10);
	output1.emplace(1333);
	output1.emplace(223);
	output1.emplace(1);


	oar(output1);

	std::set<int> input1;
	
	iar(input1);

	REQUIRE(output1 == input1);
	REQUIRE(buffer.IsFinish());

	std::unordered_set<int> output2;
	output2.emplace(10);
	output2.emplace(3333);
	output2.emplace(11);
	output2.emplace(32);
	output2.emplace(444444);

	oar(output2);
	
	std::unordered_set<int> input2;
	iar(input2);

	REQUIRE(output2 == input2);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Player")
{
	Player output1(111);

	oar(output1);

	Player intput1;
	iar(intput1);

	REQUIRE(output1.id == intput1.id);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Command")
{
	Command output1;
	output1.x_axis = 100;
	output1.y_axis = 100;

	oar(output1);

	Command intput1;
	iar(intput1);

	REQUIRE(output1.x_axis == intput1.x_axis);
	REQUIRE(output1.y_axis == intput1.y_axis);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "SnapshotPerformance")
{


}