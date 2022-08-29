local GA_HeavyAttackB = {
	cooldown_gameplay_effect = "",
	cost_gameplay_effect = "",
	ability_tags = { 
		GameplayTagDefine.kAbilityMeleeAttackHeavyB
	},
	cancel_ability = {},
	block_abilites_with_tag = {
	},
	activation_owned_tags = {
		GameplayTagDefine.kStateAttack,
		GameplayTagDefine.kStateBlockMove,
	},
	activation_required_tags = {
		GameplayTagDefine.kStateMeleeAttackHeavyA,
		GameplayTagDefine.kStateGrounded,
	},
	activation_blocked_tags =
	{
		GameplayTagDefine.kStateHurt,
		GameplayTagDefine.kStateSprint,
		GameplayAttribute.kStateJump
	},
	source_required_tags = {},
	source_blocked_tags = {},
	target_required_tags = {},
	target_blocked_tags = {},
}

return GA_HeavyAttackB