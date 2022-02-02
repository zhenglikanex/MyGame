local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    
    self:CreateAnimBone()
    self:StartHitTest()

    GameplayPlayAnimAndWait(self.entity,"skill100040","AnimCompleted","AnimCanceled")
    GameplayWaitEvent(self.entity,"GA.StartInputTask","OnStartInputTask")
    GameplayWaitEvent(self.entity,"GA.OverInputTask","OnOverInputTask")
end

function AttackGAScript:StartHitTest()
    GameplayWait(self.entity,0,"OnHitTest")
end

function AttackGAScript:OnHitTest()
    local v = self.spec.entity_manager:find(1)
    if v then
        local weapon = GetWeapon(self.spec.source)
        if weapon then
            GameplayHitTest(self.entity,1,weapon.collider_info,GetTransform(v),"OnHit")
        end
    end
    
    self:StartHitTest()
end

function AttackGAScript:OnHit(hits)
    for _,e in pairs(hits) do
        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= self.spec.source then
            ActorChangeState(e)
            ApplyCueToTarget(e,"Hurt")
        end
    end
end

function AttackGAScript:AnimCompleted()
    self:DesotryAnimBone()
    self:EndAbility()
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
end

function AttackGAScript:OnStartInputTask()
    GameplayInput(self.entity,"j","OnKeyPressed","OnKeyReleased")
end

function AttackGAScript:OnOverInputTask()
    RemoveGameplayInput(self.entity,"j")
end

function AttackGAScript:OnKeyPressed()
    --TryGameplayAbility(self.spec.source,"AttackGA2",1,EnttNull)
    self:EndAbility()
end

function AttackGAScript:OnKeyReleased()
end

return AttackGAScript