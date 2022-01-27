#pragma once

#include <cstdint>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct ModifierAttribute
{
	UniqueId source;
	UniqueId target;
	GameplayAttributeType attribute;
	GameplayEffect::ModifierOp modifier_op;
	float value;

	ModifierAttribute() = default;
	ModifierAttribute(UniqueId _source, UniqueId _target, GameplayAttributeType _attribute,float _value)
		: source(_source)
		, target(_target)
		, attribute(_attribute)
		, value(_value)
	{

	}
};

struct InfiniteModifierAttribute
{
	std::string name;
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

struct InfiniteModifierAttributeSet
{
	std::array<std::vector<InfiniteModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};