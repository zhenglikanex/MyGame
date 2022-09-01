#pragma once

#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/Service/FileService.hpp"

#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Character.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Framework/Game/Component/GAS/ActiveGameplayAbilityList.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/ActorConfig.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

struct CreateActorSystem : public System
{
	const std::string path = "Config/Actor/";
	std::unordered_map<std::string, ActorInfo> actor_infos;

	explicit CreateActorSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		//todo:预加载一下
		GetActorInfo("SuperHero.json");
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<Local,ActorAsset>(entt::exclude<ViewAsset>);
		for (auto e : view)
		{
			const auto& asset = view.get<ActorAsset>(e);

			auto& actor_info = GetActorInfo(asset.value);

			registry.emplace<ViewAsset>(e, actor_info.model_asset());
			registry.emplace<AnimationAsset>(e, actor_info.anim_asset());
			registry.emplace<Weapon>(e, actor_info.weapon());
			registry.emplace<Collider>(e, actor_info.body());

			registry.emplace<TransformState>(e);
			registry.emplace<GameplayState>(e);

			// character controller相关
			registry.emplace<MovementForward>(e, vec3(0, 0, 1.f));
			registry.emplace<Character>(e);
			registry.emplace<CharacterMovement>(e);

			// GAS
			registry.emplace<GameplayAttributeSet>(e);
			registry.emplace<InfiniteModifierAttributeSet>(e);
			registry.emplace<GameplayTagCountContainer>(e);

			//todo:随便写死试试
			auto& abilitys = registry.emplace<ActiveGameplayAbilityList>(e);
			abilitys.value.emplace("GA_Roll", ActiveGameplayAbility("GA_Roll", 1));
			abilitys.value.emplace("GA_Jump", ActiveGameplayAbility("GA_Jump", 1));
			abilitys.value.emplace("GA_DoubleJump", ActiveGameplayAbility("GA_DoubleJump", 1));
			abilitys.value.emplace("GA_Sprint", ActiveGameplayAbility("GA_Sprint", 1));
			abilitys.value.emplace("GA_LightAttackA", ActiveGameplayAbility("GA_LightAttackA", 1));
			abilitys.value.emplace("GA_LightAttackB", ActiveGameplayAbility("GA_LightAttackB", 1));
			abilitys.value.emplace("GA_LightAttackC", ActiveGameplayAbility("GA_LightAttackC", 1));
			abilitys.value.emplace("GA_HeavyAttackA", ActiveGameplayAbility("GA_HeavyAttackA", 1));
			abilitys.value.emplace("GA_HeavyAttackB", ActiveGameplayAbility("GA_HeavyAttackB", 1));
			abilitys.value.emplace("GA_HeavyAttackC", ActiveGameplayAbility("GA_HeavyAttackC", 1));
			abilitys.value.emplace("GA_HeavyAttackD", ActiveGameplayAbility("GA_HeavyAttackD", 1));
			abilitys.value.emplace("GA_DashLightAttack", ActiveGameplayAbility("GA_DashLightAttack", 1));
			abilitys.value.emplace("GA_DashHeavyAttack", ActiveGameplayAbility("GA_DashHeavyAttack", 1));
			abilitys.value.emplace("GA_JumpAttackA", ActiveGameplayAbility("GA_JumpAttackA", 1));
			abilitys.value.emplace("GA_HuoQiu", ActiveGameplayAbility("GA_HuoQiu", 1));
			abilitys.value.emplace("GA_LeiFa", ActiveGameplayAbility("GA_LeiFa", 1));

			gameplay_effect_utility::ApplyGameEffectToTarget(registry, e, actor_info.ge(), 1, e);
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
