#pragma once

#include <type_traits>
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

#include "3rdparty/include/entt/entt.hpp"

class Timer
{
public:
	Timer()
	{
		start_ = std::chrono::steady_clock::now();
	}

	double Elapsed()
	{
		return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_).count();
	}
private:
	std::chrono::steady_clock::time_point start_;
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


int main()
{
	std::cout << std::numeric_limits<uint16_t>::max() << std::endl;
	std::cout << (1 << 16) << std::endl;
	std::string a;
	std::string_view sv = a;
	std::unordered_map<std::string, int> mmp;
	mmp.find(sv.data());

	kanex::Buffer buffer{ 36744 };
	kanex::BinaryStream stream{ buffer };
	kanex::BinaryOutputArchive oar{ stream };
	kanex::BinaryInputArchive iar{ stream };

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

	{
		AutoTimer t("snapshot");
		for (int i = 0; i < 1000; ++i)
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
			Health>(oar);
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
			Health
			>(oar);
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
			Health>(oar);
	}

	entt::registry reg;
	reg.reserve<AnimationClip,
		Movement,
		Transform,
		ColliderInfo,
		SkillState,
		Collider,
		ActorState,
		AttributeUnitList,
		Health>(500);
	reg.reserve(500);

	{
		AutoTimer t("load");
		entt::snapshot_loader{ reg }.entities(iar)
			.component<
			AnimationClip,
			Movement,
			Transform,
			ColliderInfo,
			SkillState,
			Collider,
			ActorState,
			AttributeUnitList,
			Health>(iar);
	}

	system("pause");
	return 0;
}