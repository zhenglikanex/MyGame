#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/GameHelper.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ActorState.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Character.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/ActorConfig.hpp"

#include "Framework/Game/Utility/ActorStateUtility.hpp"
#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

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
		auto& game_helper = registry.ctx<Locator>().Ref<const GameHelper>();
		auto view = registry.view<ActorAsset>(entt::exclude<ViewAsset>);
		for (auto e : view)
		{
			const auto& asset = view.get<ActorAsset>(e);

			auto& actor_info = GetActorInfo(asset.value);
			registry.emplace<ViewAsset>(e, actor_info.model_asset());
			registry.emplace<AnimationAsset>(e, actor_info.anim_asset());
			registry.emplace<Weapon>(e, actor_info.weapon());
			registry.emplace<ColliderInfo>(e, actor_info.body());

			if (registry.has<TransformState>(e))
			{
				// character controller相关
				registry.emplace<MovementForward>(e);
				registry.emplace<Character>(e);
				registry.emplace<CharacterMovement>(e);

				ActionStateUtility::ChangeState(registry, e, ActorStateType::kIdle);
			}

			if (registry.has<GameplayState>(e))
			{
				GameplayEffectUtility::ApplyGameEffectToTarget(registry, e, actor_info.ge(), 1, e);
			}
			//INFO("create actor");
		}
	}

	void Finalize() override
	{

	}

	// todo:
	// 应该单独拿出来做个Manager，方便预加载资源，偷个懒
	//1. 如果作为一个单例则服务器会有并发问题(可以考虑无锁实现)
	//2. 如果作为一个实例实现则会存在多份资源
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
