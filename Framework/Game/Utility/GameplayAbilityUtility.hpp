#pragma once

#include "Framework/Game/GAS/GameplayAbility.hpp"
#include "Framework/Game/Component/LocalId.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/GAS/TryGameplayAbility.hpp"

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

	static void LuanchGameplayAbility(entt::registry& registry,Player player)
	{
		auto e = registry.create();
		//registry.emplace<LocalId>();
	}
};