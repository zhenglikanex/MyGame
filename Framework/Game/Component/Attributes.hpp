#pragma once

#include <vector>
#include <array>

#include "Framework/Game/Math.hpp"

#include "3rdparty/include/entt/entt.hpp"

enum class CalculateType : uint8_t
{
	kNumerical,			// 数值,直接进行加减
	kPercentage,		// 百分比,用当前的数据进行百分比计算
};

enum class AttributeType
{
	kHp,
	kMp,
	kDamage,
	kMax
};

struct Attribute
{
	CalculateType calc_type;
	fixed16 value;
};

//表示来自某个系统的属性集合
struct AttributeUnit
{
	entt::entity owner;
	std::array<Attribute, (size_t)AttributeType::kMax> attributes;
};

//将各个系统的属性挂到这个下面
struct AttributeUnitList
{
	std::vector<AttributeUnit> value;
};