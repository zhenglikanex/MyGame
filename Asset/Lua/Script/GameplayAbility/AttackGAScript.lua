local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    self:CreateAnimBone()
    GameplayWait(self.entity,0,"OnHitTest")
    GameplayPlayAnimAndWait(self.entity,"skill100010","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
end

function AttackGAScript:OnHitTest()
    local v = self.spec.entity_manager:find(1)
    if v then
        Destory(v)
        self.spec.entity_manager:erase(1)
    end
    GameplayHitTest(self.entity,1,)
    GameplayWait(self.entity,0,"OnHitTest")
end

function AttackGAScript:AnimCompleted()
    self:DesotryWeaponCollider()
end

function AttackGAScript:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGAScript:CreateAnimBone()
    local value = CreateAnimBone(self.spec.source,"RightWeapon")
    if value then
        self.spec.entity_manager:add(1,value)     
    end
end

function AttackGAScript:DesotryAnimBone()
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
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGAScript:OnOverInputTask()
    RemoveGameplayInput(self.entity,"j")
end

function AttackGAScript:OnKeyPressed()
    TryGameplayAbility(self.spec.source,"AttackGA2",1,EnttNull)
    self:EndAbility()
end

function AttackGAScript:OnKeyReleased()
end

return AttackGAScript