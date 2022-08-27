local GA_MelleAttackScript = require("GameplayAbility/GA_MeleeAttackScript")
local GA_HeavyAttackDScript = class("GA_LightAttackAScript",GA_MelleAttackScript)

GA_HeavyAttackDScript.animation = "HeavyAttackD"

return GA_HeavyAttackDScript

