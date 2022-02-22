local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_LightAttackAScript = class("GA_LightAttackAScript",GA_MelleAttackScript)

GA_LightAttackAScript.animation = "LightAttackA"

--todo简单点不从用动作事件了,因为懒得做没做编辑器 懒
GA_LightAttackAScript.granted_tag = GameplayTagDefine.kStateMeleeAttackLightA
GA_LightAttackAScript.granted_time = 0.3
GA_LightAttackAScript.remove_time = 1

return GA_LightAttackAScript

