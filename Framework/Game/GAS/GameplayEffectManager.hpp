#pragma once

#include <string>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "3rdparty/include/entt/entt.hpp"
#include "3rdparty/include/sol/sol.hpp"

class GameplayEffectManager
{
public:
	GameplayEffectManager(entt::registry& registry)
		: registry_(registry)
	{

	}

	const GameplayEffect& LoadGameplayEffect(const std::string& name)
	{
		auto it = gameplay_effects_.find(name);
		if (it != gameplay_effects_.end())
		{
			return (it->second);
		}

		gameplay_effects_.emplace(name, GameplayEffect());
		auto& gameplay_effect = gameplay_effects_[name];

		auto& lua = registry_.ctx<sol::state>();
		lua["LoadGameplayEffect"](name,gameplay_effect);

		return gameplay_effect;
	}

private:
	entt::registry& registry_;
	std::unordered_map<std::string, GameplayEffect> gameplay_effects_;
};