#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/SkillGraphAsset.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Attributes.hpp"
#include "Framework/Game/Component/Health.hpp"
#include "Framework/Game/Component/Collider.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/ActorConfig.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"

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

	void FixedUpdate(fixed16 dt) override
	{
		auto view = registry.view<ActorAsset>(entt::exclude<ViewAsset>);
		for (auto e : view)
		{
			const auto& asset = view.get<ActorAsset>(e);

			auto& actor_info = GetActorInfo(asset.value);
			registry.emplace<ModifyHealthList>(e);
			registry.emplace<ViewAsset>(e, actor_info.model_asset());
			registry.emplace<AnimationAsset>(e, actor_info.anim_asset());
			registry.emplace<Weapon>(e, actor_info.weapon());

			if (!actor_info.skill_graph_asset().empty())
			{
				registry.emplace<SkillGraphAsset>(e, actor_info.skill_graph_asset());
			}

			registry.emplace<ColliderInfo>(e, actor_info.body());

			//todo ����actor�������Ժ���������
			AttributeArray attributes{ Attribute{ CalculateType::kNumerical,fixed16(100)},Attribute{CalculateType::kNumerical,fixed16(100) },Attribute{CalculateType::kNumerical,fixed16(2)} };
			auto& attribute_units = registry.emplace<AttributeUnitList>(e);
			attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo actor
			attribute_units.value.emplace_back(AttributeUnit{ e, attributes });	// todo wepon

			ActionStateUtility::ChangeState(registry, e, ActorStateType::kIdle);	

			//INFO("create actor");
		}
	}

	void Finalize() override
	{

	}

	// todo:
	// Ӧ�õ����ó�������Manager������Ԥ������Դ��͵����
	//1. �����Ϊһ����������������в�������(���Կ�������ʵ��)
	//2. �����Ϊһ��ʵ��ʵ�������ڶ����Դ
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
