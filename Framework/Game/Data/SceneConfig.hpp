#pragma once

#include <vector>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Json.hpp"

#include "Framework/Game/Component/ColliderInfo.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct Obstacle
{
	friend void from_json(const nlohmann::json& json, Obstacle& info);

	ColliderInfo collider;
	mat4 transform;
};

inline void from_json(const nlohmann::json& json, Obstacle& info)
{
	json.at("collider").get_to(info.collider);
	json.at("transform").get_to(info.transform);
}

class SceneInfo
{
public:
	friend void from_json(const nlohmann::json& json, SceneInfo& info);

	SceneInfo() = default;

	const std::vector<Obstacle>& obstacles() const { return obstacles_; }
private:
	std::vector<Obstacle> obstacles_;
};

inline void from_json(const nlohmann::json& json, SceneInfo& info)
{
	json.at("obstacles").get_to(info.obstacles_);
}