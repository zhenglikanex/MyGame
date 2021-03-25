#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/Math.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/SkillGraphAsset.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Attributes.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/ActorConfig.hpp"

#include "Framework/Game/Utility/ActorUtility.hpp"

struct CreateActorSystem : public System
{
	const std::string path = "Config/Actor/";
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

			auto& actor_info = GetActorInfo(asset.value);
			registry.emplace<ViewAsset>(e, actor_info.model_asset());
			registry.emplace<AnimationAsset>(e, actor_info.anim_asset());

			if (actor_info.skill_graph_asset().empty())
			{
				registry.emplace<SkillGraphAsset>(e, actor_info.skill_graph_asset());
			}

			auto& info = registry.emplace<ColliderInfo>(e, actor_info.body());
			info.collider = CreateBodyCollider(e, info);

			registry.emplace<Weapon>(e, actor_info.weapon());

			//todo 插入actor自身属性和武器属性
			AttributeArray attributes{ Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
			auto& attribute_units = registry.emplace<AttributeUnitList>(e);
			attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo actor
			attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo wepon
		}
	}

	entt::entity CreateBodyCollider(entt::entity e,ColliderInfo& info)
	{
		auto collider = registry.create();
		registry.emplace<Collider>(collider, info.geometry, info.trigger,e);
		return collider;
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
			auto content = locator.Ref<FileService>().OpenFile(path + name);
			
			assert(content != "" && "not actor info");

			nlohmann::json j = nlohmann::json::parse(content);
			auto ret = actor_infos.emplace(name, j.get<ActorInfo>());

			return ret.first->second;
		}
	}

};
