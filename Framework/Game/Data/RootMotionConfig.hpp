#pragma once

#include <unordered_map>

#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Data/Config.hpp"
#include "Framework/Game/Math.hpp"

//struct RootMotion
//{
//	RootMotion() : delta_position(fixed16(0)), delta_rotation(fixed16(0)) {}
//	vec3 delta_position;
//	vec3 delta_rotation;
//};
//
//inline void from_json(const nlohmann::json& j, RootMotion& root_motion)
//{
//	INFO("from_json deltaPosition", j.at("deltaPosition").dump());
//	INFO("from_json deltaRotation", j.at("deltaRotation").dump());
//	j.at("deltaPosition").get_to(root_motion.delta_position);
//	j.at("deltaRotation").get_to(root_motion.delta_rotation);
//}
//
//using RootMotionConfig = Config<std::string, fixed16, RootMotion>;

//namespace nlohmann
//{
//	namespace detail
//	{
//		template<>
//		struct is_constructible_object_type<json, std::unordered_map<fixed16, RootMotion>> : std::true_type {};
//	}
//}

//nlohmann::detail::is_constructible_object_type<json, std::unordered_map<std::string, RootMotion>>::value;