#pragma once

#include "Framework/Game/Component/Attributes.hpp"

enum class ModifyType
{
	kNumerical,			// 数值,直接进行加减
	kNowPercentage,		// 百分比,用当前的数据进行百分比计算
	kTotalPercentage,	// 百分比,用总的数据进行百分比计算
};

struct Health
{
	// 如果变换频繁的话可以相关东西都放到脚本中
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