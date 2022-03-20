#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityCommand.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayAbilityUtility
{
public:
	static void BuildGameplayAbilityCommand(
		Registry& registry,
		entt::entity source,
		std::string_view ability,
		entt::entity target = entt::null)
	{
		auto& try_ga = registry.ctx_or_set<::GameplayAbilityCommand>();
		if (try_ga.value.find(source) == try_ga.value.end())
		{
			try_ga.value.emplace(source, std::vector<GameplayAbilityCommandSpec>());
		}
		try_ga.value[source].emplace_back(ability, source, target);
	}

	static void BuildGameplayAbilityCommand(
		Registry& registry,
		Remote source,
		std::string_view ability,
		Remote target = Remote::null)
	{
		GameplayAbilityUtility::BuildGameplayAbilityCommand(registry,registry.get(source), ability, registry.get(target));
	}

	static void CommitGameplayAbilityCost(Registry& registry,entt::entity ability)
	{
		if (registry.valid(ability))
		{
			auto& spec = registry.get<GameplayAbilitySpec>(ability);
			
			entt::entity source = registry.get(spec.source);

			if (source == entt::null)
			{
				return;
			}

			auto& ga_manager = registry.ctx<GameplayAbilityManager>();
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);

			GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, ga.cost_gameplay_effect, spec.level, source);
		}
	}

	static void CommitGameplayAbilityCooldown(Registry& registry, entt::entity ability)
	{
		if (registry.valid(ability))
		{
			auto& spec = registry.get<GameplayAbilitySpec>(ability);

			entt::entity source = registry.get(spec.source);
			
			if (source == entt::null)
			{
				return;
			}

			auto& ga_manager = registry.ctx<GameplayAbilityManager>();
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);

			if (ga.cooldown_duration <= 0)
			{
				return;
			}

			auto effect_spec = GameplayEffectUtility::MakeGameplayEffectSpec(registry, source, ga.cooldown_gameplay_effect,spec.level, source);
			effect_spec.duration = ga.cooldown_duration;

			for (auto it = ga.cooldown_tags.Begin(); it != ga.cooldown_tags.End(); ++it)
			{
				effect_spec.dynamic_granted_tags.Push(*it);
			}

			GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, std::move(effect_spec));
		}
	}

	static std::vector<entt::entity> GetActiveGameplayAbilities(Registry& registry, const Remote& id)
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