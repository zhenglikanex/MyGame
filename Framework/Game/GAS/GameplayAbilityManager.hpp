#pragma once

#include <string>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "3rdparty/include/entt/entt.hpp"
#include "3rdparty/include/sol/sol.hpp"

class GameplayAbilityManager
{
public:
	GameplayAbilityManager(entt::registry& registry)
		: registry_(registry)
	{

	}

	const GameplayAbility& LoadGameplayAbility(const std::string& name)
	{
		auto it = gameplay_abilities_.find(name);
		if (it != gameplay_abilities_.end())
		{
			return it->second;
		}

		gameplay_abilities_.emplace(name, GameplayAbility());
		auto& gameplay_ability = gameplay_abilities_[name];

		auto& lua = registry_.ctx<sol::state>();
		lua["LoadGameplayAbility"](name, gameplay_ability);

		return gameplay_ability;
	}

private:
	entt::registry& registry_;
	std::unordered_map<std::string, GameplayAbility> gameplay_abilities_;
};