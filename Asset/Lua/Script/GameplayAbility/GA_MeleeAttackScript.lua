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
    local spec = self:GetSpec()
    for _,e in pairs(hits) do
        print("GA_MeleeAttackScript:OnHit(hits)",e)

        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= spec.source then
            ApplyGameplayEffectToTarget(GetEntity(spec.source),"GE_Attack",spec.level,e)
            ApplyCueToTarget(e,"Hurt")
        end
    end
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