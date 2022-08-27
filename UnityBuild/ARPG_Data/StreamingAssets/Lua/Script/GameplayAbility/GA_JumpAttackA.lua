local GA_JumpAttack = {
	cooldown_gameplay_effect = "",
	cost_gameplay_effect = "",
	ability_tags = { 
		GameplayTagDefine.kAbilityMeleeAttackJumpA 
	},
	cancel_ability = {
		
	},
	block_abilites_with_tag = {
	},
	activation_owned_tags = {
		GameplayTagDefine.kStateAttack,
		GameplayTagDefine.kStateBlockMove
	},
	activation_required_tags = 
	{
		
	},
	activation_blocked_tags =
	{
		GameplayTagDefine.kStateAttack,
		GameplayTagDefine.kStateGrounded,
		GameplayTagDefine.kStateAttack,
	},
	source_required_tags = {},
	source_blocked_tags = {},
	target_required_tags = {},
	target_blocked_tags = {},
}

return GA_JumpAttack