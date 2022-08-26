local GAScript = require("GAS/GAScript")
local GA_MeleeAttackScript = class("GA_MeleeAttackScript",GAScript)

GA_MeleeAttackScript.animation = "LightAttackA"
GA_MeleeAttackScript.hit_tag = nil

function GA_MeleeAttackScript:ActiveAbility()
    self:CommitAbility()
    
    self:CreateAnimBone()
    self:StartHitTest()

    GameplayAbilityTimeline(self.entity,self.animation,"TimelineCompleted")
end

function GA_MeleeAttackScript:OnHit(hits)
    local spec = self:GetSpec()
    for _,e in pairs(hits) do
        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= spec.source then
            --ApplyCueToTarget(e,"Hurt")
        end
    end
end

function GA_MeleeAttackScript:TimelineCompleted()
    self:EndAbility()
end

function GA_MeleeAttackScript:EndAbility()
    GA_MeleeAttackScript.super.EndAbility(self)
end

return GA_MeleeAttackScript