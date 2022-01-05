#pragma once

#include <unordered_map>

#include "Framework/Game/Component/Local.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct EntityByLocalMap
{
	std::unordered_map<Local, entt::entity> value;
};
