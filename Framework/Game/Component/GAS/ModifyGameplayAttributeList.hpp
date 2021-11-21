#pragma once

#include <cstdint>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct BaseModifierAttribute
{
	GameplayAttributeType attribute;
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct BaseModifierAttributeList
{
	std::vector<BaseModifierAttribute> value;
};

struct ModifierAttribute
{
	GameplayAttributeType attribute;
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct ModifierAttributeSet
{
	std::array<std::vector<ModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};

void RegisterModifier(sol::state& lua)
{
	
}