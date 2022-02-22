local GAScript = require("GAS/GAScript")
local GA_MeleeAttackScript = class("GA_MeleeAttackScript",GAScript)

GA_MeleeAttackScript.animation = nil
GA_MeleeAttackScript.hit_tag = nil

--todo简单点不从用动作事件了,因为懒得做没做编辑器 懒
GA_MeleeAttackScript.granted_tag = nil
GA_MeleeAttackScript.granted_time = nil
GA_MeleeAttackScript.remove_time = nil

function GA_MeleeAttackScript:ActiveAbility()
    self:CommitAbility()
    
    self:CreateAnimBone()
    self:StartHitTest()

    if self.granted_tag and self.granted_time and self.remove_time then
        GameplayWait(self.entity,self.granted_time,"OnGrantedTag")
        GameplayWait(self.entity,self.remove_time,"OnRemoveTag")
    end

    GameplayPlayAnimAndWait(self.entity,self.animation,"AnimCompleted","AnimCanceled")
end

function GA_MeleeAttackScript:StartHitTest()
    GameplayWait(self.entity,0,"OnHitTest")
end

function GA_MeleeAttackScript:OnHitTest()
    local spec = self:GetSpec()
    local v = spec.entity_manager:find(1)
    if v then
        local weapon = GetWeapon(spec)
        if weapon then
            GameplayHitTest(self.entity,1,weapon.collider,GetTransform(v),"OnHit")
        end
    end
    
    self:StartHitTest()
end

function GA_MeleeAttackScript:OnHit(hits)
    local spec = self:GetSpec()
    for _,e in pairs(hits) do
        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= spec.source then
            ApplyCueToTarget(e,"Hurt")
        end
    end
end

function GA_MeleeAttackScript:AnimCompleted()
    self:EndAbility()
end

function GA_MeleeAttackScript:AnimCanceled()
    self:EndAbility()
end

function GA_MeleeAttackScript:CreateAnimBone()
    local spec = self:GetSpec()

    local value = CreateAnimBone(spec.source,"RightWeapon")
    if value then
        spec.entity_manager:add(1,value)     
    end
end

function GA_MeleeAttackScript:DesotryAnimBone()
    local spec = self:GetSpec()
    local v = spec.entity_manager:find(1)
    if v then
        Destory(v)
        spec.entity_manager:erase(1)
    end
end

function GA_MeleeAttackScript:OnGrantedTag()
    local spec = self:GetSpec()
    local source = GetEntity(spec.source)
    if source ~= EnttNull then
        local tag_count = GetGameplayTagCountContainer(source)
        tag_count:UpdateTagCount(self.granted_tag,1)
    end
end

function GA_MeleeAttackScript:OnRemoveTag()
    if self.granted_tag then
        local spec = self:GetSpec()
        local source = GetEntity(spec.source)
        if source ~= EnttNull then
            dump(debug.traceback())
            local tag_count = GetGameplayTagCountContainer(source)
            tag_count:UpdateTagCount(self.granted_tag,-1)
        end
    end
end

function GA_MeleeAttackScript:EndAbility()
    self:DesotryAnimBone()
    self:OnRemoveTag()
    GA_MeleeAttackScript.super.EndAbility(self)
end

return GA_MeleeAttackScript