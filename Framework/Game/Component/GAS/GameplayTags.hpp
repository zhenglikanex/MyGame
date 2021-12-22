#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

#include "Kanex.hpp"

struct GameplayTags
{
	GameplayTagArray tags;
	GameplayTagArray immunity_tags;
};

NON_MEMEBER_BAR(GameplayTags, obj.tags, obj.immunity_tags);