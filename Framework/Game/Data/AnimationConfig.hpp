#pragma once

#include <unordered_map>
#include <string>
#include <unordered_map>
#include <mutex>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Math.hpp"
#include "Framework/Game/GameConfig.hpp"

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

struct AnimEvent
{
	float time;
	std::string name;
};

struct AnimationClipInfo
{
	fixed16 length;
	std::vector<RootMotion> root_motions;
	std::vector<Skeleton> skeletons;
	std::unordered_map<uint32_t,std::vector<AnimEvent>> anim_events_map;
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

inline void from_json(const nlohmann::json& j, AnimEvent& anim_event)
{
	j.at("time").get_to(anim_event.time);
	j.at("name").get_to(anim_event.name);
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

	if (j.contains("events"))
	{
		auto& anim_events = j.at("events").get<std::vector<AnimEvent>>();
		for (auto& event : anim_events)
		{
			uint32_t frame = event.time / static_cast<float>(GameConfig::kFrameTime);
			clip_info.anim_events_map[frame].emplace_back(event);
		}
	}
}

inline void from_json(const nlohmann::json& j, AnimationInfo& animation_info)
{
	auto& clips = j.at("clips");
	animation_info.clips.reserve(clips.size());
	clips.get_to(animation_info.clips);
}
