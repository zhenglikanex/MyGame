local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_HeavyAttackBScript = class("GA_HeavyAttackBScript",GA_MelleAttackScript)

GA_HeavyAttackBScript.animation = "HeavyAttackB"
GA_HeavyAttackBScript.granted_tag = GameplayTagDefine.kStateMeleeAttackHeavyB
GA_HeavyAttackBScript.granted_time = 0.3
GA_HeavyAttackBScript.remove_time = 1

return GA_HeavyAttackBScript

