#pragma once

#include <cstdint>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"

struct ModifierAttribute
{
	entt::entity target;
	GameplayAttributeType attribute;
	GameplayEffect::ModifierOp modifier_op;
	float value;

	ModifierAttribute() = default;
	ModifierAttribute(entt::entity _target, GameplayAttributeType _attribute, GameplayEffect::ModifierOp _modifier_op, float _value)
		: target(_target)
		, attribute(_attribute)
		, modifier_op(_modifier_op)
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

struct ExecutionAttribute
{
	entt::entity target;
	std::string ge;

	ExecutionAttribute() = default;

	ExecutionAttribute(entt::entity _target, std::string_view _ge)
		: target(_target)
		, ge(_ge)
	{

	}

};