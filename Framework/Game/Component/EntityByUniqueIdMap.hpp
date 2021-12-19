#pragma once

#include <unordered_map>

#include "Framework/Game/Component/UniqueId.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct EntityByUniqueIdMap
{
	std::unordered_map<UniqueId, entt::entity> value;
};