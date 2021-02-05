#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/Weapon.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/ActorConfig.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

struct CreateActorSystem : public System
{
	std::unordered_map<std::string, ActorInfo> actor_infos;

	CreateActorSystem(entt::registry& _registry) : System(_registry) { }
	~CreateActorSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<ActorAsset>(entt::exclude<ViewAsset>);
		for (auto e : view)
		{
			const auto& asset = view.get<ActorAsset>(e);
			auto actor_info = GetActorInfo(asset.value);
			registry.emplace<ViewAsset>(e, actor_info.model_asset);
			registry.emplace<AnimationAsset>(e, actor_info.anim_asset);
			registry.emplace<BoundingBox>(e, actor_info.body);
			registry.emplace<Weapon>(e, actor_info.weapon);
			ActionStateUtility::ChangeState(registry, e, ActorStateType::kIdle);
		}
	}

	void Finalize() override
	{

	}

	const ActorInfo& GetActorInfo(const std::string& name)
	{
		auto iter = actor_infos.find(name);
		if (iter != actor_infos.end())
		{
			return iter->second;
		}
		else
		{
			auto& locator = registry.ctx<Locator>();
			auto content = locator.Ref<FileService>().OpenFile(name);
			
			assert(content != "" && "not actor info");

			nlohmann::json j = nlohmann::json::parse(content);
			ActorInfo info(j.at("modelAsset"), j.at("animAsset"), GetBoundingBox(j.at("bodyCollision")), GetBoundingBox(j.at("weaponCollision")));
			actor_infos.emplace(name, std::move(info));

			return actor_infos[name];
		}
	}
};
