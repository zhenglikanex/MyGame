local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    --ReceiveGameplayEvent(self.entity,"GA.AttackGA",function(target)
        --print("this is ",target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --self:EndAbility()
    --end)
    GameplayPlayAnimAndWait(self.entity,"skill100010","AnimCompleted","AnimCanceled")
    GameplayWaitTask(self.entity,0.3,"CreateWeaponCollider")
    GameplayWaitTask(self.entity,0.5,"")
end

function AttackGAScript:AnimCompleted()
    print("this is AnimCompleted")
end

function AttackGAScript:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGAScript:CreateWeaponCollider()
    local weapon = GetWeapon(self.sepc.source)
    if weapon then
        self.spec.entity_manager:insert(1,CreateColliderToAnimBone(self.spec.source, weapon.collider_info, "RightWeapon"))
    end     
end

function AttackGAScript:DesotryWeaponCollider()
    
end

return AttackGAScript