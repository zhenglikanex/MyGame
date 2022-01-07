local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    --self:EndAbility()
    --ReceiveGameplayEvent(self.entity,"GA.AttackGA",function(target)
        --print("this is ",target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --self:EndAbility()
    --end)
    GameplayPlayAnimAndWait(self.entity,"skill100010","AnimCompleted","AnimCanceled")
    self:CreateWeaponCollider()
end

function AttackGAScript:AnimCompleted()
    self:DesotryWeaponCollider()
end

function AttackGAScript:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGAScript:CreateWeaponCollider()
    local weapon = GetWeapon(self.spec.source)
    if weapon then
        local value = CreateColliderToAnimBone(weapon.collider_info,self.spec.source,"RightWeapon",self.loc)
        self.spec.entity_manager:add(1,value)
    end     
end

function AttackGAScript:DesotryWeaponCollider()
    local v = self.spec.entity_manager:find(1)
    if v then
        Destory(v)
        self.spec.entity_manager:erase(1)
    end

    self:EndAbility()
end

function AttackGAScript:OnTrigger(target)
    local unique_id = GetUniqueId(target)
    if unique_id and unique_id ~= self.spec.source then
       print("AttackGAScript:OnTrigger(target)!!!!!!!!") 
       ActorChangeState(unique_id)
    end
end

return AttackGAScript