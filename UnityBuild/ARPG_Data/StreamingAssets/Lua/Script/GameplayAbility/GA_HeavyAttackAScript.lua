local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_HeavyAttackAScript = class("GA_LightAttackAScript",GA_MelleAttackScript)

GA_HeavyAttackAScript.animation = "HeavyAttackA"

return GA_HeavyAttackAScript

