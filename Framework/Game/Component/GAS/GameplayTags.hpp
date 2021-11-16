#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "3rdparty/include/sol/sol.hpp"

struct GameplayTags
{
	std::vector<std::string> self;
	std::unordered_map<std::string, std::vector<std::string>> ge_tags;
};
