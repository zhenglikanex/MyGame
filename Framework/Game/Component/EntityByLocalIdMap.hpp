#pragma once

#include <unordered_map>

#include "3rdparty/include/entt/entt.hpp"

struct EntityByLocalIdMap
{
	std::unordered_map<uint32_t, entt::entity> value;
};