#pragma once

#include <string>
#include <unordered_map>

// 拥有的技能信息
struct ActiveGameplayAbility
{
	std::string ability;
	uint32_t level;

	ActiveGameplayAbility() = default;

	ActiveGameplayAbility(std::string_view _ability,uint32_t _level)
		: ability(_ability)
		, level(_level)
	{

	}
};

struct ActiveGameplayAbilityList
{
	std::unordered_map<std::string, ActiveGameplayAbility> value;
};
