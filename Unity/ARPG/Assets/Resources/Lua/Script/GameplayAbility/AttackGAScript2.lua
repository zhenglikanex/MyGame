local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    self:CreateWeaponCollider()

    GameplayPlayAnimAndWait(self.entity,"skill100020","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
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
       ActorChangeState(unique_id)
       ApplyCueToTarget(unique_id,"Hurt")
    end
end

function AttackGAScript:OnStartInputTask()
    print("---------------OnStartInputTask")
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGAScript:OnOverInputTask()
    print("---------------OnOverInputTask")
    RemoveGameplayInput(self.entity,"j")
end

function AttackGAScript:OnKeyPressed()
    print("function AttackGAScript:OnKeyPressed()")
    --self:EndAbility()
    --TryGameplayAbility(self.spec.source,"AttackGA2",1,EnttNull)
end

function AttackGAScript:OnKeyReleased()
    print("function AttackGAScript:OnKeyReleased()")
end

return AttackGAScript