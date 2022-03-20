#pragma once

#include <cstdint>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

#include "Kanex.hpp"

struct ModifierAttribute
{
	Remote source;
	Remote target;
	GameplayAttributeType attribute;
	GameplayEffect::ModifierOp modifier_op;
	float value;

	ModifierAttribute() = default;
	ModifierAttribute(Remote _source, Remote _target, GameplayAttributeType _attribute,float _value)
		: source(_source)
		, target(_target)
		, attribute(_attribute)
		, value(_value)
	{

	}
};

NON_MEMEBER_BAR(ModifierAttribute, obj.source, obj.target, obj.attribute, obj.modifier_op, obj.value);

struct InfiniteModifierAttribute
{
	std::string name;
	GameplayEffect::ModifierOp modifier_op;
	float value;
};

NON_MEMEBER_BAR(InfiniteModifierAttribute, obj.name, obj.modifier_op, obj.value);

struct InfiniteModifierAttributeSet
{
	std::array<std::vector<InfiniteModifierAttribute>, (size_t)GameplayAttributeType::MaxCount> value;
};

NON_MEMEBER_BAR(InfiniteModifierAttributeSet, obj.value);
