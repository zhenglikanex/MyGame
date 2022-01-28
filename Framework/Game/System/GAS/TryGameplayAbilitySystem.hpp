#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/GameHelper.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/DebugService.hpp"

#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/ActiveGameplayAbilityList.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"

struct TryGameplayAbilitySystem : public System
{
	TryGameplayAbilitySystem(entt::registry& _registry) : System(_registry) { }
	~TryGameplayAbilitySystem() {}

	bool Initialize() override
	{
		return true;
	}

	const ActiveGameplayAbility* GetActiveAbility(entt::entity e,const std::string& ability)
	{
		auto& active_ability_list = registry.get<ActiveGameplayAbilityList>(e);
		auto it = active_ability_list.value.find(ability);
		if (it != active_ability_list.value.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	bool CheckCoolDown(entt::entity e,const GameplayAbility& ga)
	{
		auto& tags = registry.get<GameplayTags>(e);
		if (tags.tags.HasAny(ga.cooldown_tags))
		{
			return false;
		}

		return true;
	}

	bool CheckCost(entt::entity e,const GameplayAbility& ga)
	{
		if (ga.cost_gameplay_effect == "")
		{
			return true;
		}

		auto modifiers = GameplayEffectUtility::GetModifiers(registry, ga.cost_gameplay_effect);
		if (modifiers.size() > 0)
		{
			auto& attr_set = registry.get<GameplayAttributeSet>(e);
			for (auto&[attribute, value] : modifiers)
			{
				if (attr_set.value[attribute].current_value < value)
				{
					return false;
				}
			}
		}

		return true;
	}

	// 检查是否会被其他技能阻塞
	bool CheckOtherBlock(const GameplayAbility& ga,const std::vector<entt::entity>& entities)
	{
		for (auto e : entities)
		{
			auto& spec = registry.get<GameplayAbilitySpec>(e);
			
		}
	}

	void FixedUpdate(float dt) override
	{
		auto& game_helper = registry.ctx<Locator>().Ref<GameHelper>();
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();

		if (auto try_ga = registry.try_ctx<TryGameplayAbility>())
		{
			for (auto& ga_list : try_ga->value)
			{
				for (auto& info : ga_list.second)
				{
					if (!registry.valid(info.source))
					{
						continue;
					}

					if (!(registry.all_of<UniqueId>(info.source)) || !(registry.all_of<GameplayState>(info.source)))
					{
						continue;
					}

					auto active_ability = GetActiveAbility(info.source, info.ability);
					if (!active_ability)
					{
						continue;
					}

					auto& ga = ga_manager.LoadGameplayAbility(info.ability);
					auto source_tags = registry.try_get<GameplayTags>(info.source);
					//auto target_tags = registry.try_get<GameplayTags>(info.target);

					if (!CheckCoolDown(info.source,ga))
					{
						continue;
					}

					if (!CheckCost(info.source,ga))
					{
						continue;
					}

					// todo:其他检查
					UniqueId target = UniqueId::null;
					if (info.target != entt::null)
					{
						target = registry.get<UniqueId>(info.target);
					}

					auto e = registry.create();
					registry.emplace<GameplayAbilitySpec>(e, info.ability,active_ability->level,registry.get<UniqueId>(info.source),target);
					if (game_helper.IsAuthority())
					{
						registry.emplace<UniqueId>(e, e);
					}
					if (auto player = registry.try_get<Player>(info.source))
					{
						// 玩家控制角色
						auto& sync_manager = registry.ctx<SyncManager>();
						auto& sync = sync_manager.sync_instances[player->id];
						registry.emplace<Local>(e,sync.owner,sync.identifier++);
					}
					else
					{
						//服务器控制角色
						auto& sync_manager = registry.ctx<SyncManager>();
						auto& sync = sync_manager.sync_instances[std::numeric_limits<uint32_t>::max()];
						registry.emplace<Local>(e, sync.owner, sync.identifier++);
					}
				}

				ga_list.second.clear();
			}
		}
	}
	

	void Finalize() override
	{

	}
};
