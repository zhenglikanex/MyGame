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

class ActorInfoManager
{
public:
	ActorInfoManager(entt::registry& registry)
		: registry_(registry)
	{

	}

	const ActorInfo& GetActorInfo(const std::string& name)
	{
#ifdef SERVER
		// todo:偷懒,服务器应该提供一个无锁的实现,暂时加个锁
		std::unique_lock<std::mutex> lock(mutex_);
#endif
		auto iter = actor_infos_.find(name);
		if (iter != actor_infos_.end())
		{
			return iter->second;
		}
		else
		{
			auto& locator = registry_.ctx<Locator>();
			auto content = locator.Ref<FileService>().OpenFile(path + name);

			assert(content != "" && "not actor info");

			nlohmann::json j = nlohmann::json::parse(content);
			auto ret = actor_infos_.emplace(name, j.get<ActorInfo>());

			return ret.first->second;
		}
	}
private:
#ifdef SERVER
	// todo:偷懒,服务器应该体统一个无锁的实现,暂时加个锁
	std::mutex mutex_;
#endif
	const std::string path = "Config/Actor/";
	std::unordered_map<std::string, ActorInfo> actor_infos_;
	entt::registry& registry_;
};