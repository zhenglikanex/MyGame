#pragma once

#include <cstdint>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct ModifierAttribute
{
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct InfiniteModifierAttribute
{
	std::string name;
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct ModifierBaseAttributeSet
{
	std::array<std::vector<ModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};

struct InfiniteModifierAttributeSet
{
	std::array<std::vector<InfiniteModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};