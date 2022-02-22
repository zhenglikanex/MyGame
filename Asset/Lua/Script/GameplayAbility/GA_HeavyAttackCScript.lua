local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_HeavyAttackCScript = class("GA_HeavyAttackCScript",GA_MelleAttackScript)

GA_HeavyAttackCScript.animation = "HeavyAttackC"
GA_HeavyAttackCScript.granted_tag = GameplayTagDefine.kStateMeleeAttackHeavyC
GA_HeavyAttackCScript.granted_time = 0.3
GA_HeavyAttackCScript.remove_time = 1

return GA_HeavyAttackCScript

