local GAScript = require("GAS/GAScript")
local AttackGA3Script = class("AttackGA3Script",GAScript)

function AttackGA3Script:ActiveAbility()
    self:CommitAbility()
    self:CreateWeaponCollider()

    GameplayPlayAnimAndWait(self.entity,"skill100030","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
end

function AttackGA3Script:AnimCompleted()
    self:DesotryWeaponCollider()
end

function AttackGA3Script:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGA3Script:CreateWeaponCollider()
    local weapon = GetWeapon(self.spec.source)
    if weapon then
        local value = CreateColliderToAnimBone(weapon.collider_info,self.spec.source,"RightWeapon",self.loc)
        self.spec.entity_manager:add(1,value)
    end     
end

function AttackGA3Script:DesotryWeaponCollider()
    local v = self.spec.entity_manager:find(1)
    if v then
        Destory(v)
        self.spec.entity_manager:erase(1)
    end
    self:EndAbility()
end

function AttackGA3Script:OnTrigger(target)
    local unique_id = GetUniqueId(target)
    if unique_id and unique_id ~= self.spec.source then
       ActorChangeState(unique_id)
       ApplyCueToTarget(unique_id,"Hurt")
    end
end

function AttackGA3Script:OnStartInputTask()
    print("---------------OnStartInputTask")
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGA3Script:OnOverInputTask()
    print("---------------OnOverInputTask")
    RemoveGameplayInput(self.entity,"j")
end

function AttackGA3Script:OnKeyPressed()
    print("function AttackGA3Script:OnKeyPressed()")
    --self:EndAbility()
    --TryGameplayAbility(self.spec.source,"AttackGA2",1,EnttNull)
end

function AttackGA3Script:OnKeyReleased()
    print("function AttackGA3Script:OnKeyReleased()")
end

return AttackGA3Script