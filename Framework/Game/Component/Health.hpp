#pragma once

#include "Framework/Game/Component/Attributes.hpp"

enum class ModifyType
{
	kNumerical,			// ��ֵ,ֱ�ӽ��мӼ�
	kNowPercentage,		// �ٷֱ�,�õ�ǰ�����ݽ��аٷֱȼ���
	kTotalPercentage,	// �ٷֱ�,���ܵ����ݽ��аٷֱȼ���
};

struct Health
{
	// ����任Ƶ���Ļ�������ض������ŵ��ű���
	AttributeArray cur;
	AttributeArray max;

	Health() = default;
	Health(const AttributeArray& _cur, const AttributeArray& _max)
		: cur(_cur)
		, max(_max)
	{

	}

	Health(AttributeArray&& _cur, const AttributeArray& _max)
		: cur(std::move(_cur))
		, max(_max)
	{

	}

	Health(const AttributeArray& _cur, AttributeArray&& _max)
		: cur(_cur)
		, max(std::move(_max))
	{

	}

	Health(AttributeArray&& _cur, AttributeArray&& _max)
		: cur(std::move(_cur))
		, max(std::move(_max))
	{

	}
};

struct ModifyHealth
{
	AttributeType attribute_type;
	ModifyType modify_type;
	fixed16 value;

	ModifyHealth() = default;

	ModifyHealth(AttributeType _attribute_type, ModifyType _modify_type, fixed16 _value)
		: attribute_type(_attribute_type)
		, modify_type(_modify_type)
		, value(_value)
	{

	}
};

struct ModifyHealthList
{
	std::vector<ModifyHealth> value;

	ModifyHealthList() = default;

	ModifyHealthList(const std::vector<ModifyHealth>& _value)
		: value(_value)
	{

	}

	ModifyHealthList(std::vector<ModifyHealth>&& _value)
		: value(std::move(_value))
	{

	}
};

NON_MEMEBER_BAR(Health, obj.cur, obj.max);
NON_MEMEBER_BAR(ModifyHealth, obj.attribute_type, obj.modify_type, obj.value);
NON_MEMEBER_BAR(ModifyHealthList, obj.value);