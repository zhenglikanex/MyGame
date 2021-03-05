#pragma once

#include <vector>
#include <array>

#include "Framework/Game/Math.hpp"

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

//��ʾ����ĳ��ϵͳ�����Լ���
struct AttributeUnit
{
	entt::entity owner;
	std::array<Attribute, (size_t)AttributeType::kMax> attributes;
};

//������ϵͳ�����Թҵ��������
struct AttributeUnitList
{
	std::vector<AttributeUnit> value;
};