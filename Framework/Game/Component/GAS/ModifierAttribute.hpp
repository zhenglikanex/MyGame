#pragma once

#include <cstdint>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct ModifierAttribute
{
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct ModifierBaseAttributeSet
{
	std::array<std::vector<ModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};

struct ModifierAttributeSet
{
	std::array<std::vector<ModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};