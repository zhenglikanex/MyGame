#pragma once

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayAbility
{
	std::string cooldown_gameplay_effect;
	std::string cost_gameplay_effect;

	GameplayTagArray ability_tags;				//GameplayAbility拥有的GameplayTag, 这只是用来描述GameplayAbility的GameplayTag.
	GameplayTagArray cancel_ability;			//当该GameplayAbility激活时, 其他Ability Tags中拥有这些GameplayTag的GameplayAbility将会被取消.
	GameplayTagArray block_abilites_with_tag;	//当该GameplayAbility激活时, 其他Ability Tags中拥有这些GameplayTag的GameplayAbility将会阻塞激活.
	GameplayTagArray activation_owned_tags;		//当该GameplayAbility激活时, 这些GameplayTag会交给该GameplayAbility的拥有者.
	GameplayTagArray activation_required_tags;	//该GameplayAbility只有在其拥有者拥有所有这些GameplayTag时才会激活.
	GameplayTagArray activation_blocked_tags;	//该GameplayAbility在其拥有者拥有任意这些标签时不能被激活.
	GameplayTagArray source_required_tags;		//该GameplayAbility只有在Source拥有所有这些GameplayTag时才会激活. Source GameplayTag只有在该GameplayAbility由Event触发时设置.
	GameplayTagArray source_blocked_tags;		//该GameplayAbility在Source拥有任意这些标签时不能被激活. Source GameplayTag只有在该GameplayAbility由Event触发时设置.
	GameplayTagArray target_required_tags;		//该GameplayAbility只有在Target拥有所有这些GameplayTag时才会激活. Target GameplayTag只有在该GameplayAbility由Event触发时设置.
	GameplayTagArray target_blocked_tags;		//该GameplayAbility在Target拥有任意这些标签时不能被激活. Target GameplayTag只有在该GameplayAbility由Event触发时设置.
	
	float cooldown_duration;					//冷却持续时间
	GameplayTagArray cooldown_tags;				//冷却的tag
	
	sol::table script;
};
