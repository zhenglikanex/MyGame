#pragma once

#include <string>
#include <unordered_map>

#include "Framework/Game/Registry.hpp"
#include "Framework/Game/GAS/GameplayAbilityTimeline.hpp"
#include "Framework/Game/Service/FileService.hpp"

class GameplayAbilityTimelineManager
{
public:
	inline static const std::string path = "Config/Timeline/";

	GameplayAbilityTimelineManager(Registry& registry)
		: registry_(registry)
	{

	}

	const GameplayAbilityTimeline& LoadGameplayAbilityTimeline(const std::string& name)
	{
		auto it = gameplay_ability_timeline_.find(name);
		if (it != gameplay_ability_timeline_.end())
		{
			return it->second;
		}

		auto& locator = registry_.ctx<Locator>();
		auto content = locator.Ref<FileService>().OpenFile(path + name + ".json");

		assert(content != "" && "not actor info");

		nlohmann::json j = nlohmann::json::parse(content);
		gameplay_ability_timeline_.emplace(name, j.get<GameplayAbilityTimeline>());

		auto& timeline = gameplay_ability_timeline_[name];
		timeline.RegisterEvents();

		return timeline;
	}
private:
	Registry& registry_;
	std::unordered_map<std::string,GameplayAbilityTimeline> gameplay_ability_timeline_;
};
