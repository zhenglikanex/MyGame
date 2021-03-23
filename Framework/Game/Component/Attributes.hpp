#pragma once

#include <vector>
#include <array>

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

enum class CalculateType : uint8_t
{
	kNumerical,			// ��ֵ,ֱ�ӽ��мӼ�
	kPercentage,		// �ٷֱ�,�õ�ǰ�����ݽ��аٷֱȼ���
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

using AttributeArray = std::array<Attribute, (size_t)AttributeType::kMax>;

//��ʾ����ĳ��ϵͳ�����Լ���
struct AttributeUnit
{
	entt::entity owner;
	AttributeArray attributes;

	AttributeUnit() = default;
	
	AttributeUnit(entt::entity _owner, const AttributeArray& _attributes)
		: owner(_owner)
		, attributes(_attributes)
	{

	}

	AttributeUnit(entt::entity _owner, AttributeArray&& _attributes)
		: owner(_owner)
		, attributes(std::move(_attributes))
	{

	}
};

//������ϵͳ�����Թҵ��������
struct AttributeUnitList
{
	std::vector<AttributeUnit> value;

	AttributeUnitList() = default;

	AttributeUnitList(const std::vector<AttributeUnit>& _value)
		:value(_value)
	{

	}

	AttributeUnitList(std::vector<AttributeUnit>&& _value)
		: value(std::move(_value))
	{

	}
};

NON_MEMEBER_BAR(Attribute, obj.calc_type, obj.value);
NON_MEMEBER_BAR(AttributeUnit, obj.owner, obj.attributes);
NON_MEMEBER_BAR(AttributeUnitList, obj.value);