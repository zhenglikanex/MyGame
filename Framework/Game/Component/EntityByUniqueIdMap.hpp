#pragma once

#include <unordered_map>

#include "Framework/Game/Component/Remote.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct EntityByUniqueIdMap
{
	std::unordered_map<Remote, entt::entity> value;
};