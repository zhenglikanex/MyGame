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
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Health.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct ActorUtiltiy
{
	static const std::string path;
	static std::unordered_map<std::string,ActorInfo> actor_infos;

	static entt::entity CreatePlayer(entt::registry& reg,uint32_t id, const std::string& actor_asset,const vec3& position = vec3(0,0,0),const quat& rotation = glm::identity<quat>())
	{
		auto e = reg.create();

		reg.emplace<Player>(e,id);
		reg.emplace<Transform>(e, position, rotation);
		reg.emplace<ModifyHealthList>(e);
		ActionStateUtility::ChangeState(reg, e, ActorStateType::kIdle);

		auto& actor_info = GetActorInfo(reg, actor_asset);
		reg.emplace<ViewAsset>(e, actor_info.model_asset());
		reg.emplace<AnimationAsset>(e, actor_info.anim_asset());
		reg.emplace<Weapon>(e, actor_info.weapon());

		if (actor_info.skill_graph_asset().empty())
		{
			reg.emplace<SkillGraphAsset>(e, actor_info.skill_graph_asset());
		}

		auto& info = reg.emplace<ColliderInfo>(e, actor_info.body());
		info.collider = CreateBodyCollider(reg, e, info);

		//todo 插入actor自身属性和武器属性
		AttributeArray attributes{ Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
		auto& attribute_units = reg.emplace<AttributeUnitList>(e);
		attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo actor
		attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo wepon
	}

	static entt::entity CreateMonster(entt::registry& reg, uint32_t id, const std::string& actore_asset, const vec3& position = vec3(0, 0, 0), const quat& rotation = glm::identity<quat>())
	{

	}
private:
	static entt::entity CreateBodyCollider(entt::registry& registry,entt::entity e, ColliderInfo& info)
	{
		auto collider = registry.create();
		registry.emplace<Collider>(collider, info.geometry, info.trigger, e);
		return collider;
	}

 	static const ActorInfo& GetActorInfo(entt::registry& registry, const std::string& name)
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

const std::string ActorUtiltiy::path = "Config/Actor/";