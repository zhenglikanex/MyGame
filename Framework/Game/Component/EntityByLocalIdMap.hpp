#pragma once

#include <unordered_map>

#include "Framework/Game/Component/Local.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct EntityByLocalIdMap
{
	std::unordered_map<LocalId, entt::entity> value;
};
