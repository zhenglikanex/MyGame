#define CATCH_CONFIG_MAIN

#include "3rdparty/include/catch2/catch.hpp"
#include "Framework/Game/Math.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/Movement.hpp"
//#include "Framework/Game/Component/Transform.hpp"

struct Serialize
{
	kanex::Buffer buffer{ 1000 };
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

	
	mat4 src3 = glm::identity<mat4>();
	oar(src3);
}

TEST_CASE_METHOD(Serialize, "AnimationClip")
{
	
	AnimationClip src("123");
	src.is_done = true;
	src.time = fixed16(19);
	
	oar(src);

	AnimationClip out1;
	iar(out1);

	REQUIRE(src.time == out1.time);
	REQUIRE(src.is_done == out1.is_done);
	REQUIRE(src.name == src.name);

	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "ActoreState")
{

	ActorState src1(ActorStateType::kAttack);
	src1.time = fixed16(10);

	EnterActorState src2(ActorStateType::kDeath);
	ExitActorState src3(ActorStateType::kHurt);

	oar(src1);
	oar(src2);
	oar(src3);

	ActorState out1;
	EnterActorState out2;
	ExitActorState out3;
	
	iar(out1);
	iar(out2);
	iar(out3);

	REQUIRE(src1.cur_state == out1.cur_state);
	REQUIRE(src1.time == out1.time);
	REQUIRE(src2.value == out2.value);
	REQUIRE(src3.value == out3.value);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Movement")
{
	Movement src(vec3(100, 200, 3), vec3(10, 20, 30.3), vec3(30, 20, 20));

	oar(src);
	
	Movement out;

	iar(out);

	REQUIRE(src.forward == out.forward);
	REQUIRE(src.velocity == out.velocity);
	REQUIRE(src.angular_velocity == out.angular_velocity);
	REQUIRE(buffer.IsFinish());
}

//TEST_CASE_METHOD(Serialize,"Transform")
//{
//	Transform output(vec3(0.222,0.333,0.1),quat(10,20,30,40));
//	
//	//oar(output);
//	
//	Transform input;
//	//iar(input);
//
//	REQUIRE(output.position == input.position);
//	REQUIRE(output.rotation == output.rotation);
//	REQUIRE(buffer.IsFinish());
//}
