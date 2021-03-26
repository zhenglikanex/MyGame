#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Json.hpp"

#include "Framework/Game/Component/ColliderInfo.hpp"

#include "3rdparty/include/entt/entt.hpp"

class ActorInfo
{
public:
	friend void from_json(const nlohmann::json& json, ActorInfo& info);

	ActorInfo() = default;

	const std::string& model_asset() const { return model_asset_; }
	const std::string& anim_asset() const { return anim_asset_; }
	const std::string& skill_graph_asset() const { return anim_asset_; }
	const ColliderInfo& body() const { return body_; }
	const ColliderInfo& weapon() const { return weapon_; }
private:
	std::string model_asset_;
	std::string anim_asset_;
	ColliderInfo body_;
	ColliderInfo weapon_;
};

inline void from_json(const nlohmann::json& json, ActorInfo& info)
{
	json.at("modelAsset").get_to(info.model_asset_);
	json.at("animAsset").get_to(info.anim_asset_);
	json.at("bodyCollision").get_to(info.body_);
	json.at("weaponCollision").get_to(info.weapon_);
}