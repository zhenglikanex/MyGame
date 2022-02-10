#pragma once

#include "Framework/Game/GAS/GameplayTag.hpp"

// todo这个状态定义可以通过编辑器去生成
// lua和C++都生成一份
namespace GameplayTagDefine
{
	GameplayTag kStatus("Status");
	GameplayTag kAbility("Ability");
	GameplayTag kAbilityJump = kAbility + "Jump";
	GameplayTag kAbilityMeleeAttack = kAbility + "";
	const std::string kAbility = "Ability";

	//Status
	namespace Status
	{
		GameplayTag kHurt(kStatus + "." + "Hurt");
	}

	//Ability
	namespace Ability
	{
		GameplayTag kJump(kAbility + "Jump");
		GameplayTag kMeleeAttack(kAbility + "." + "MeleeAttack");
	}
}