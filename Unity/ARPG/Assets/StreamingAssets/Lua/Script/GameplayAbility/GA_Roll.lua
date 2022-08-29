local GA_Roll = {
    cooldown_gameplay_effect = "DefaultCooldownGE",
	cost_gameplay_effect = "",
	ability_tags = { GameplayTagDefine.kAbilityRoll },
	cancel_ability = { },
	block_abilites_with_tag = { GameplayTagDefine.kAbilityJump,GameplayTagDefine.kAbilitySprint,GameplayTagDefine.kAbilityRoll },
	activation_owned_tags = { GameplayTagDefine.kStateRoll,GameplayTagDefine.kStateBlockMove },
	activation_required_tags = { GameplayTagDefine.kStateGrounded },
	activation_blocked_tags ={},
	source_required_tags = {},
	source_blocked_tags = {},
	target_required_tags = {},
	target_blocked_tags = {},

	cooldown_duration = 10,
	cooldown_tags = { GameplayTagDefine.kCooldownRoll },
}

return GA_Roll