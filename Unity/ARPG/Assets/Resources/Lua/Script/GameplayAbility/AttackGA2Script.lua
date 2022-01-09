local GAScript = require("GAS/GAScript")
local AttackGA2Script = class("AttackGA2Script",GAScript)

function AttackGA2Script:ActiveAbility()
    self:CommitAbility()
    self:CreateWeaponCollider()

    GameplayPlayAnimAndWait(self.entity,"skill100020","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
end

function AttackGA2Script:AnimCompleted()
    self:DesotryWeaponCollider()
end

function AttackGA2Script:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGA2Script:CreateWeaponCollider()
    local weapon = GetWeapon(self.spec.source)
    if weapon then
        local value = CreateColliderToAnimBone(weapon.collider_info,self.spec.source,"RightWeapon",self.loc)
        self.spec.entity_manager:add(1,value)
    end     
end

function AttackGA2Script:DesotryWeaponCollider()
    local v = self.spec.entity_manager:find(1)
    if v then
        Destory(v)
        self.spec.entity_manager:erase(1)
    end
    self:EndAbility()
end

function AttackGA2Script:OnTrigger(target)
    local unique_id = GetUniqueId(target)
    if unique_id and unique_id ~= self.spec.source then
       ActorChangeState(unique_id)
       ApplyCueToTarget(unique_id,"Hurt")
    end
end

function AttackGA2Script:OnStartInputTask()
    print("---------------OnStartInputTask")
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGA2Script:OnOverInputTask()
    print("---------------OnOverInputTask")
    RemoveGameplayInput(self.entity,"j")
end

function AttackGA2Script:OnKeyPressed()
    print("function AttackGA2Script:OnKeyPressed()")
    print(self,self.spec,self.entity)
    dump(self)
    TryGameplayAbility(self.spec.source,"AttackGA3",1,EnttNull)
    self:EndAbility()
end

function AttackGA2Script:OnKeyReleased()
    print("function AttackGA2Script:OnKeyReleased()")
end

return AttackGA2Script