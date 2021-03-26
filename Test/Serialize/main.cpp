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
#include "Framework/Game/Component/Contact.hpp"
#include "Framework/Game/Component/Attributes.hpp"
#include "Framework/Game/Component/Health.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Skill.hpp"
#include "Framework/Game/Component/SkillState.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Matrix4x4.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/SkillGraphAsset.hpp"

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

TEST_CASE_METHOD(Serialize, "SkillGraphAsset")
{
	SkillGraphAsset output("131231231");

	oar(output);

	SkillGraphAsset input;

	iar(input);

	REQUIRE(output.value == input.value);
	REQUIRE(buffer.IsFinish());
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

TEST_CASE_METHOD(Serialize, "Transform")
{
	std::vector<Transform> outvec;
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
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Weapon")
{
	ColliderInfo info(Geometry(Sphere(10)), glm::identity<mat4>(), true, entt::entity(300));
	Weapon output(info);
	output.attributes.emplace_back(Attribute{ CalculateType::kNumerical,fixed16(100) });
	oar(output);

	Weapon input;
	iar(input);

	REQUIRE(output.collider_info.geometry.type() == input.collider_info.geometry.type());
	REQUIRE(output.collider_info.transform == input.collider_info.transform);
	REQUIRE(output.collider_info.trigger == input.collider_info.trigger);
	REQUIRE(output.attributes[0].calc_type == input.attributes[0].calc_type);
	REQUIRE(output.attributes[0].value == input.attributes[0].value);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Contact")
{
	ContactList output;
	output.value.push_back(Contact{ (entt::entity)10 });
	output.value.push_back(Contact{ (entt::entity)31 });
	output.value.push_back(Contact{ (entt::entity)20 });

	oar(output);

	ContactList input;
	iar(input);

	REQUIRE(output.value[0].entity == input.value[0].entity);
	REQUIRE(output.value[1].entity == input.value[1].entity);
	REQUIRE(output.value[2].entity == input.value[2].entity);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Attributes")
{
	AttributeUnitList output;
	AttributeArray attributes{ Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
	output.value.emplace_back(entt::null, attributes);
	output.value.emplace_back((entt::entity)100, attributes);
	
	oar(output);

	AttributeUnitList input;
	iar(input);
	

	for (int i = 0; i < output.value.size(); ++i)
	{
		REQUIRE(input.value[i].owner == output.value[i].owner);
		for (int j = 0; j < output.value[i].attributes.size(); ++j)
		{
			REQUIRE(input.value[i].attributes[i].calc_type == output.value[i].attributes[i].calc_type);
			REQUIRE(input.value[i].attributes[i].value == output.value[i].attributes[i].value);
		}
	}

	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Health")
{
	Health output;
	output.cur = { Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
	output.max = { Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };

	oar(output);
	
	Health input;
	iar(input);

	for (int i = 0; i < output.cur.size(); ++i)
	{
		REQUIRE(input.cur[i].calc_type == output.cur[i].calc_type);
		REQUIRE(input.cur[i].value == output.cur[i].value);

		REQUIRE(input.max[i].calc_type == output.max[i].calc_type);
		REQUIRE(input.max[i].value == output.max[i].value);
	}

	REQUIRE(buffer.IsFinish());

	ModifyHealthList output1;
	output1.value.emplace_back(AttributeType::kHp, ModifyType::kNowPercentage, 10);
	output1.value.emplace_back(AttributeType::kDamage, ModifyType::kNumerical, 0.32);

	oar(output1);

	ModifyHealthList input1;
	iar(input1);

	REQUIRE(output1.value.size() == input1.value.size());
	for (int i = 0; i < output1.value.size(); ++i)
	{
		REQUIRE(output1.value[i].attribute_type == input1.value[i].attribute_type);
		REQUIRE(output1.value[i].modify_type == input1.value[i].modify_type);
		REQUIRE(output1.value[i].value == input1.value[i].value);
	}
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "Collider")
{
	Collider output1(Geometry(Sphere(10)), true, entt::entity(10));
	Collider output2(Geometry(OBB(vec3(50))), false, entt::entity(10));
	Collider output3(Geometry(Capsule(10.2, 0.3)), true, entt::entity(10));	
	oar(output1);
	oar(output2);
	oar(output3);

	Collider input1;
	Collider input2;
	Collider input3;

	iar(input1);
	iar(input2);
	iar(input3);

	REQUIRE(output1.geometry.type() == input1.geometry.type());
	REQUIRE(output1.geometry.sphere().r == input1.geometry.sphere().r);
	REQUIRE(output1.trigger == input1.trigger);
	REQUIRE(output1.owner == input1.owner);

	REQUIRE(output2.geometry.type() == input2.geometry.type());
	REQUIRE(output2.geometry.box().e == input2.geometry.box().e);
	REQUIRE(output2.trigger == input2.trigger);
	REQUIRE(output2.owner == input2.owner);

	REQUIRE(output3.geometry.type() == input3.geometry.type());
	REQUIRE(output3.geometry.capsule().h == input3.geometry.capsule().h);
	REQUIRE(output3.geometry.capsule().r == input3.geometry.capsule().r);
	REQUIRE(output3.trigger == input3.trigger);
	REQUIRE(output3.owner == input3.owner);

	REQUIRE(buffer.IsFinish());
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

TEST_CASE_METHOD(Serialize, "Skill")
{
	Skill output1;
	output1.owner = entt::entity(300);
	output1.life = fixed16(10);
	output1.time = fixed16(33311.22);
	output1.targets.push_back(entt::entity(3333));
	output1.targets.push_back(entt::entity(3333));
	output1.targets.push_back(entt::entity(3322));
	output1.hit_target.emplace(entt::entity(3322));
	output1.hit_target.emplace(entt::entity(33221));
	output1.hit_target.emplace(entt::entity(33221));

	oar(output1);

	Skill input1;
	iar(input1);

	REQUIRE(output1.owner == input1.owner);
	REQUIRE(output1.life == input1.life);
	REQUIRE(output1.time == input1.time);
	REQUIRE(output1.targets == input1.targets);
	REQUIRE(output1.hit_target == input1.hit_target);
	REQUIRE(buffer.IsFinish());

	SkillAttacthBone output2("12313","3331");
	oar(output2);

	SkillAttacthBone input2;
	iar(input2);

	REQUIRE(output2.anim_name == input2.anim_name);
	REQUIRE(output2.bone_name == input2.bone_name);

	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "SkillState")
{
	SkillState output1("skilll");
	output1.time = 10;
	output1.executed_events.emplace("niubi");
	output1.executed_events.emplace("niubi2");
	output1.executed_events.emplace("niubi");

	oar(output1);
	
	SkillState input1;
	iar(input1);

	REQUIRE(output1.name == input1.name);
	REQUIRE(output1.time == input1.time);
	REQUIRE(output1.executed_events == input1.executed_events);

	REQUIRE(buffer.IsFinish());

	ExitSkillState output2("mniubi");
	oar(output2);

	ExitSkillState input2("12112");
	iar(input2);

	REQUIRE(output2.name == input2.name);
	REQUIRE(buffer.IsFinish());

	SkillGraphInfo::Param output3;
	output3.bool_value = true;
	
	SkillGraphInfo::Param output4;
	output4.int_value = 10003;

	SkillGraphInfo::Param output5;
	output5.float_value = 103.4;

	oar(output3);
	oar(output4);
	oar(output5);

	SkillGraphInfo::Param input3;
	SkillGraphInfo::Param input4;
	SkillGraphInfo::Param input5;

	iar(input3);
	iar(input4);
	iar(input5);

	REQUIRE(output3.bool_value == input3.bool_value);
	REQUIRE(output4.int_value == input4.int_value);
	REQUIRE(output5.float_value == input5.float_value);
	
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "ColliderInfo")
{
	ColliderInfo output1(Geometry(Sphere(10)), glm::identity<mat4>(), true, entt::entity(300));
	
	oar(output1);

	ColliderInfo intput1;
	iar(intput1);

	REQUIRE(output1.geometry.sphere().r == intput1.geometry.sphere().r);
	REQUIRE(output1.transform == intput1.transform);
	REQUIRE(output1.trigger == intput1.trigger);
	REQUIRE(output1.collider == intput1.collider);
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

TEST_CASE_METHOD(Serialize, "Matrix4x4")
{
	Matrix4x4 output1;

	oar(output1);

	Matrix4x4 intput1;
	iar(intput1);

	REQUIRE(output1.value == intput1.value);
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
	REQUIRE(output1.skill == intput1.skill);
	REQUIRE(output1.jump == intput1.jump);
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "entt")
{
	entt::registry registry;
	auto e1 = registry.create();
	registry.emplace<AnimationClip>(e1, "123");
	auto e2 = registry.create();
	registry.emplace<ColliderInfo>(e2,Geometry(Sphere(10)), glm::identity<mat4>(), true,e1);
	auto e3 = registry.create();
	auto e4 = registry.create();

	entt::snapshot { registry }.entities(oar).component<AnimationClip,ColliderInfo>(oar);

	entt::registry reg2;
	entt::snapshot_loader{ reg2 }
		.entities(iar)
		.component<AnimationClip, ColliderInfo>(iar);
	
	reg2;
	REQUIRE(buffer.IsFinish());
}

TEST_CASE_METHOD(Serialize, "SnapshotPerformance")
{
	entt::registry registry;
	AttributeUnitList output;
	AttributeArray attributes{ Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
	output.value.emplace_back(entt::null, attributes);
	output.value.emplace_back((entt::entity)100, attributes);

	Health health;
	health.cur = { Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
	health.max = { Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };

	Skill output1;
	output1.owner = entt::entity(300);
	output1.life = fixed16(10);
	output1.time = fixed16(33311.22);
	output1.targets.push_back(entt::entity(3333));
	output1.targets.push_back(entt::entity(3333));
	output1.targets.push_back(entt::entity(3322));
	output1.hit_target.emplace(entt::entity(3322));
	output1.hit_target.emplace(entt::entity(33221));
	output1.hit_target.emplace(entt::entity(33221));

	for (int i = 0; i < 10; ++i)
	{
		auto e = entt::handle(registry, registry.create());
		e.emplace<AnimationClip>("11222233");
		e.emplace<Movement>(vec3(100, 200, 3), vec3(10, 20, 30.3), vec3(30, 20, 20));
		e.emplace<Transform>(vec3(0.222, 0.333, 0.1), quat(10, 20, 30, 40));
		e.emplace<ColliderInfo>(Geometry(Sphere(10)), glm::identity<mat4>(), true, entt::entity(300));
		e.emplace<SkillState>("eeeee");
		e.emplace<Collider>(Geometry(Sphere(10)), true, entt::entity(10));
		e.emplace<ActorState>(ActorStateType::kAttack);
		e.emplace<AttributeUnitList>(output);
		e.emplace<Health>(health);
		e.emplace<Skill>(output1);
	}
	{
		AutoTimer t("snapshot");
		entt::snapshot{ registry }.entities(oar)
			.component<
				AnimationClip,
				Movement,
				Transform,
				ColliderInfo,
				SkillState,
				Collider,
				ActorState,
				AttributeUnitList,
				Health,
				Skill>(oar);
	}
	buffer.Clear();
	{
		AutoTimer t("snapshot");
		entt::snapshot{ registry }.entities(oar)
			.component<
			AnimationClip,
			Movement,
			Transform,
			ColliderInfo,
			SkillState,
			Collider,
			ActorState,
			AttributeUnitList,
			Health,
			Skill
			>(oar);
	}
}