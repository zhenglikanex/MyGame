#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/Locator.hpp"

#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityCommand.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/ActiveGameplayAbilityList.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAbilityCommandSystem : public System
{
	GameplayAbilityCommandSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	const ActiveGameplayAbility* GetActiveAbility(entt::entity e,const std::string& ability) const
	{
		auto& active_ability_list = registry.get<ActiveGameplayAbilityList>(e);
		auto it = active_ability_list.value.find(ability);
		if (it != active_ability_list.value.end())
		{
			return &it->second;
		}

		return nullptr;
	}

	bool CheckCoolDown(entt::entity e,const GameplayAbility& ga) const
	{
		auto& tag_count = registry.get<GameplayTagCountContainer>(e);
		if (tag_count.HasAny(ga.cooldown_tags))
		{
			return false;
		}

		return true;
	}

	bool CheckCost(entt::entity e,const GameplayAbility& ga) const
	{
		if (ga.cost_gameplay_effect == "")
		{
			return true;
		}

		auto modifiers = gameplay_effect_utility::GetModifiers(registry, ga.cost_gameplay_effect);
		if (modifiers.size() > 0)
		{
			auto& attr_set = registry.get<GameplayAttributeSet>(e);
			for (auto&[attribute, value] : modifiers)
			{
				if (attr_set.value[attribute].current_value + value < 0)
				{
					return false;
				}
			}
		}

		return true;
	}

	// 检查是否会被其他技能阻塞
	bool CheckActiveAbiliesBlocked(entt::entity source,const GameplayAbility& ga) const
	{
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
	
		auto entities = gameplay_ability_utility::GetActiveGameplayAbilities(registry, source);
		for (auto e : entities)
		{
			auto& spec = registry.get<GameplayAbilitySpec>(e);
			auto& active_ga = ga_manager.LoadGameplayAbility(spec.ability);
			if (ga.ability_tags.HasAny(active_ga.block_abilites_with_tag))
			{
				return false;
			}
		}

		return true;
	}

	bool CheckActivationRequiredTags(const GameplayTagCountContainer& tag_count,const GameplayAbility& ga) const
	{
		return tag_count.HasAll(ga.activation_required_tags);
	}

	bool CheckActivationBlockedTags(const GameplayTagCountContainer& tag_count, const GameplayAbility& ga) const
	{
		return !tag_count.HasAny(ga.activation_blocked_tags);
	}

	bool CheckTargetRequiredTags(const GameplayTagCountContainer& tag_count, const GameplayAbility& ga) const
	{
		return tag_count.HasAll(ga.target_required_tags);
	}

	bool CheckTargetBlockedTags(const GameplayTagCountContainer& tag_count, const GameplayAbility& ga) const
	{
		return !tag_count.HasAny(ga.target_blocked_tags);
	}

	void FixedUpdate(float dt) override
	{
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
		auto& command = registry.ctx_or_set<GameplayAbilityCommand>();

		for(auto& [_,command_map] : command.value)
		{
			for(auto& [_,command_spec] : command_map)
			{
				entt::entity source = registry.get(command_spec.source);

				if (!registry.valid(source))
				{
					continue;
				}

				if (!(registry.all_of<Remote>(source)) || !(registry.all_of<GameplayState>(source)))
				{
					continue;
				}

				auto& source_tag_count = registry.get<GameplayTagCountContainer>(source);

				auto active_ability = GetActiveAbility(source, command_spec.ability);
				if (!active_ability)
				{
					continue;
				}

				auto& ga = ga_manager.LoadGameplayAbility(command_spec.ability);
				if (!CheckCoolDown(source, ga))
				{
					continue;
				}

				if (!CheckCost(source, ga))
				{
					continue;
				}

				if (!CheckActiveAbiliesBlocked(source, ga))
				{
					continue;
				}

				if (!CheckActivationRequiredTags(source_tag_count, ga))
				{
					continue;
				}

				if (!CheckActivationBlockedTags(source_tag_count, ga))
				{
					continue;
				}

				if (registry.valid(command_spec.target))
				{
					entt::entity target = registry.get(command_spec.target);
					auto& target_tag_count = registry.get<GameplayTagCountContainer>(target);
					if (!CheckTargetRequiredTags(target_tag_count, ga))
					{
						continue;
					}

					if (!CheckTargetBlockedTags(target_tag_count, ga))
					{
						continue;
					}
				}

				auto e = registry.create_predict(source);
				registry.emplace<GameplayAbilitySpec>(e, command_spec.ability, active_ability->level, command_spec.source, command_spec.target);
				registry.patch<GameplayTagCountContainer>(source, [&ga](GameplayTagCountContainer& tag_count)
					{
						tag_count.UpdateTagCount(ga.activation_owned_tags, 1);
					});
			}
		}
	}
	
	void Finalize() override
	{

	}
};
