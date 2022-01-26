#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayAbilityUtility
{
public:
	static void TryGameplayAbility(
		entt::registry& registry,
		entt::entity source,
		std::string_view ability,
		entt::entity target = entt::null)
	{
		auto& try_ga = registry.ctx_or_set<::TryGameplayAbility>();
		if (try_ga.value.find(source) == try_ga.value.end())
		{
			try_ga.value.emplace(source, std::vector<GameplayAbilityInfo>());
		}
		try_ga.value[source].emplace_back(ability, source, target);
	}

	static void TryGameplayAbility(
		entt::registry& registry,
		UniqueId unique_id,
		std::string_view ability,
		entt::entity target = entt::null)
	{
		auto& map = registry.ctx<EntityByUniqueIdMap>();
		auto it = map.value.find(unique_id);
		if (it != map.value.end())
		{
			GameplayAbilityUtility::TryGameplayAbility(registry, it->second, ability, target);
		}
	}

	static void CommitGameplayAbilityCost(entt::registry& registry,entt::entity ability)
	{
		if (registry.valid(ability))
		{
			auto& spec = registry.get<GameplayAbilitySpec>(ability);
			
			auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
			auto& it = unique_map.value.find(spec.source);
			if (it == unique_map.value.end())
			{
				return;
			}

			entt::entity source = it->second;

			auto& ga_manager = registry.ctx<GameplayAbilityManager>();
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);	

			GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, ga.cost_gameplay_effect,spec.level,source);
		}
	}

	static void CommitGameplayAbilityCooldown(entt::registry& registry, entt::entity ability)
	{
		if (registry.valid(ability))
		{
			auto& spec = registry.get<GameplayAbilitySpec>(ability);

			auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
			auto& it = unique_map.value.find(spec.source);
			if (it == unique_map.value.end())
			{
				return;
			}

			entt::entity source = it->second;

			auto& ga_manager = registry.ctx<GameplayAbilityManager>();
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);

			auto effect_spec = GameplayEffectUtility::MakeGameplayEffectSpec(registry, source, ga.cost_gameplay_effect, 1, source);
			effect_spec.duration = ga.cooldown_duration;

			for (auto it = ga.cooldown_tags.Begin(); it != ga.cooldown_tags.End(); ++it)
			{
				effect_spec.dynamic_granted_tags.Push(*it);
			}

			GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, std::move(effect_spec));
		}
	}

	static std::vector<entt::entity> GetActiveGameplayAbilities(entt::registry& registry, const UniqueId& id)
	{
		std::vector<entt::entity> abilities;

		auto view = registry.view<GameplayAbilitySpec,GameplayAbilityScript>();
		for (auto e : view)
		{
			auto& spec = view.get<GameplayAbilitySpec>(e);
			if (spec.source == id)
			{
				abilities.emplace_back(e);
			}
		}

		return abilities;
	}
};