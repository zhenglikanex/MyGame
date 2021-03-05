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
	std::array<fixed16,(size_t)AttributeType::kMax> cur;
	std::array<fixed16, (size_t)AttributeType::kMax> max;
};

struct ModifyHealth
{
	AttributeType attribute_type;
	ModifyType modify_type;
	fixed16 value;
};

struct ModifyHealthList
{
	std::vector<ModifyHealth> value;
};