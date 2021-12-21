#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayTags
{
	GameplayTagArray tags;
	GameplayTagArray immunity_tags;
};