#pragma once

#include <unordered_map>
#include <string>
#include <unordered_map>
#include <mutex>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Math.hpp"

struct RootMotion
{
	fixed16 time;
	vec3 velocity;
	vec3 angular_velocity;

	RootMotion() : time(fixed16(0)),velocity(zero<vec3>()), angular_velocity(zero<vec3>()) {}
};

struct Bone
{
	mat4 transform;

	Bone() : transform(zero<mat4>()) {}
};

struct Skeleton
{
	fixed16 time;
	std::unordered_map<std::string, Bone> bones;
};

struct AnimationClipInfo
{
	fixed16 length;
	std::vector<RootMotion> root_motions;
	std::vector<Skeleton> skeletons;
};

struct AnimationInfo
{
	std::unordered_map<std::string, AnimationClipInfo> clips;
};

inline void from_json(const nlohmann::json& j, RootMotion& root_motion)
{
	j.at("time").get_to(root_motion.time);
	j.at("velocity").get_to(root_motion.velocity);
	j.at("angularVelocity").get_to(root_motion.angular_velocity);
}

inline void from_json(const nlohmann::json& j, Bone& bone)
{
	auto str = j.dump();
	j.at("transform").get_to(bone.transform);
}

inline void from_json(const nlohmann::json& j, Skeleton& skeleton)
{
	j.at("time").get_to(skeleton.time);
	j.at("bones").get_to(skeleton.bones);
}

inline void from_json(const nlohmann::json& j, AnimationClipInfo& clip_info)
{
	j.at("length").get_to(clip_info.length);

	auto& rootMotions = j.at("rootMotions");
	clip_info.root_motions.reserve(rootMotions.size());
	rootMotions.get_to(clip_info.root_motions);

	auto& skeletons = j.at("skeletons");
	clip_info.skeletons.reserve(skeletons.size());
	skeletons.get_to(clip_info.skeletons);
}

inline void from_json(const nlohmann::json& j, AnimationInfo& animation_info)
{
	auto& clips = j.at("clips");
	animation_info.clips.reserve(clips.size());
	clips.get_to(animation_info.clips);
}

class AnimationInfoManager
{
public:
	AnimationInfoManager(entt::registry registry)
		: registry_(registry)
	{

	}

	const AnimationInfo& GetAnimationInfo(const std::string& name)
	{
#ifdef SERVER
		// todo:偷懒,服务器应该提供一个无锁的实现,暂时加个锁
		std::unique_lock<std::mutex> lock(mutex_);
#endif
		auto iter = animation_infos_.find(name);
		if (iter != animation_infos_.end())
		{
			return iter->second;
		}
		else
		{
			auto& locator = registry_.ctx<Locator>();
			auto content = locator.Ref<FileService>().OpenFile(path + name);

			assert(content != "" && "not anim info");

			nlohmann::json j = nlohmann::json::parse(content);
			animation_infos_.emplace(name, j.get<AnimationInfo>());

			return animation_infos_[name];
		}
	}

private:
#ifdef SERVER
	// todo:偷懒,服务器应该体统一个无锁的实现,暂时加个锁
	std::mutex mutex_;
#endif
	const std::string path = "Config/Anim/";
	std::unordered_map<std::string, AnimationInfo> animation_infos_;
	entt::registry& registry_;
};