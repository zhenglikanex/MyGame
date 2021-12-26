#pragma once

#include <unordered_map>

#include "Framework/Game/Component/LocalId.hpp"


//记录玩家分配出去的id
struct LocalIdByPlayerMap
{
	std::unordered_map<uint32_t, LocalId> value;
};