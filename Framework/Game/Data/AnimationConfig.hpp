#pragma once

#include <unordered_map>

#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Math.hpp"

#include "Framework/Game/Data/Config.hpp"


struct RootMotion
{
	RootMotion() : delta_position(fixed16(0)), delta_rotation(fixed16(0)) {}
	vec3 delta_position;
	vec3 delta_rotation;
};

struct AnimationInfo
{
	uint32_t length;
	std::unordered_map<fixed16, RootMotion> root_motions;
};

inline void from_json(const nlohmann::json& j, RootMotion& root_motion)
{
	j.at("deltaPosition").get_to(root_motion.delta_position);
	j.at("deltaRotation").get_to(root_motion.delta_rotation);
}

inline void from_json(const nlohmann::json& j, AnimationInfo& animation_info)
{
	j.at("length").get_to(animation_info.length);
	j.at("root_motions").get_to(animation_info.root_motions);
}

/*
 * key1 模型名字
 * key2 动画名字
 */
using AnimationConfig = Config<std::string, std::string, AnimationInfo>;