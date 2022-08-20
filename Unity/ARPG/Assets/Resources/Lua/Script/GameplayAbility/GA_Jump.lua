local GA_Jump = 
{
	cooldown_gameplay_effect = "",
	cost_gameplay_effect = "",
	ability_tags = { GameplayTagDefine.kAbilityJump },
	cancel_ability = {  },
	block_abilites_with_tag = { GameplayTagDefine.kAbilityJump },
	activation_owned_tags = { GameplayTagDefine.kStateJump,GameplayTagDefine.kStateBlockMove },
	activation_required_tags = { GameplayTagDefine.kStateGrounded },
	activation_blocked_tags ={  },
	source_required_tags = {},
	source_blocked_tags = {},
	target_required_tags = {},
	target_blocked_tags = {},
}

return GA_Jump