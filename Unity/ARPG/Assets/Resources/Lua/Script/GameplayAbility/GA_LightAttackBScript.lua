local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_LightAttackBScript = class("GA_LightAttackBScript",GA_MelleAttackScript)

GA_LightAttackBScript.animation = "LightAttackB"
GA_LightAttackBScript.granted_tag = GameplayTagDefine.kStateMeleeAttackLightB
GA_LightAttackBScript.granted_time = 0.3
GA_LightAttackBScript.remove_time = 1

return GA_LightAttackBScript

