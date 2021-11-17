#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayTags
{
	std::vector<GameplayTag> self;
	std::unordered_map<std::string, std::vector<GameplayTag>> ge_tags;

	void ApplyGETags(const std::string& ge)
	{
		ge_tags.emplace(ge, std::vector<GameplayTag>());
	}
};

void RegisterGameplayTags(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTags>("GameplayTags");
	type["self"] = &GameplayTags::self;
	type["ge_tags"] = &GameplayTags::ge_tags;
	type["ApplyGETags"] = &GameplayTags::ApplyGETags;
}
