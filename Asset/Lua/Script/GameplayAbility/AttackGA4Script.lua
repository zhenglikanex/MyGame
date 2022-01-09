local GAScript = require("GAS/GAScript")
local AttackGA4Script = class("AttackGA4Script",GAScript)

function AttackGA4Script:ActiveAbility()
    self:CommitAbility()
    self:CreateWeaponCollider()

    GameplayPlayAnimAndWait(self.entity,"skill100010","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
end

function AttackGA4Script:AnimCompleted()
    self:DesotryWeaponCollider()
end

function AttackGA4Script:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGA4Script:CreateWeaponCollider()
    local weapon = GetWeapon(self.spec.source)
    if weapon then
        local value = CreateColliderToAnimBone(weapon.collider_info,self.spec.source,"RightWeapon",self.loc)
        self.spec.entity_manager:add(1,value)
    end     
end

function AttackGA4Script:DesotryWeaponCollider()
    local v = self.spec.entity_manager:find(1)
    if v then
        Destory(v)
        self.spec.entity_manager:erase(1)
    end
    self:EndAbility()
end

function AttackGA4Script:OnTrigger(target)
    local unique_id = GetUniqueId(target)
    if unique_id and unique_id ~= self.spec.source then
       ActorChangeState(unique_id)
       ApplyCueToTarget(unique_id,"Hurt")
    end
end

function AttackGA4Script:OnStartInputTask()
    print("---------------OnStartInputTask")
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGA4Script:OnOverInputTask()
    print("---------------OnOverInputTask")
    RemoveGameplayInput(self.entity,"j")
end

function AttackGA4Script:OnKeyPressed()
    print("function AttackGA4Script:OnKeyPressed()")
    --self:EndAbility()
    --TryGameplayAbility(self.spec.source,"AttackGA2",1,EnttNull)
end

function AttackGA4Script:OnKeyReleased()
    print("function AttackGA4Script:OnKeyReleased()")
end

return AttackGA4Script