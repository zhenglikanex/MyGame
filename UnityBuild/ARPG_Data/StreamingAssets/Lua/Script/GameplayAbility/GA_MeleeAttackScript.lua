local GAScript = require("GAS/GAScript")
local GA_MeleeAttackScript = class("GA_MeleeAttackScript",GAScript)

GA_MeleeAttackScript.animation = nil
GA_MeleeAttackScript.hit_tag = nil

function GA_MeleeAttackScript:ActiveAbility()
    self:CommitAbility()
    
    GameplayAbilityTimeline(self.entity,self.animation,"TimelineCompleted")
    GameplayAnimWait(self.entity,"","AnimCanceled")
end

function GA_MeleeAttackScript:OnHit(hits)
    
end

function GA_MeleeAttackScript:TimelineCompleted()
    self:EndAbility()

end

function GA_MeleeAttackScript:EndAbility()
    GA_MeleeAttackScript.super.EndAbility(self)
end

function GA_MeleeAttackScript:AnimCanceled()
    self:EndAbility()
end

return GA_MeleeAttackScript