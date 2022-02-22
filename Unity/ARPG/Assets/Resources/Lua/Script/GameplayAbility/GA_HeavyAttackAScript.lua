local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_HeavyAttackAScript = class("GA_LightAttackAScript",GA_MelleAttackScript)

GA_HeavyAttackAScript.animation = "HeavyAttackA"
GA_HeavyAttackAScript.granted_tag = GameplayTagDefine.kStateMeleeAttackHeavyA
GA_HeavyAttackAScript.granted_time = 0.3
GA_HeavyAttackAScript.remove_time = 1

return GA_HeavyAttackAScript

