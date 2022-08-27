local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_DashLightAttackScript = class("GA_DashLightAttackScript",GA_MelleAttackScript)

GA_DashLightAttackScript.animation = "DashHeavyAttack"

return GA_DashLightAttackScript

