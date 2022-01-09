#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"

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

	static void LuanchGameplayAbility(entt::registry& registry,Player player)
	{
		auto e = registry.create();
		//registry.emplace<LocalId>();
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