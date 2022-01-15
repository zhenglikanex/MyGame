local GAScript = require("GAS/GAScript")
local HuoQiuGAScript = class("HuoQiuGAScript",GAScript)

function HuoQiuGAScript:ActiveAbility()
    self:CommitAbility()
    self:CreateHuoqiue()
    GameplayWait(self.entity,5,"EndAbility")
end

function HuoQiuGAScript:CreateHuoqiue()
    self.huoqiu = CreateMovementView(self.spec.source,"HuoQiu",GetTransform(self.spec.source),vec3.new(0,0,1),vec3.new(0,0,1),vec3.new(0,0,0))
    self:StartHitTest()
end

function HuoQiuGAScript:StartHitTest()
    GameplayWait(self.entity,0,"OnHitTest")
end

function HuoQiuGAScript:OnHitTest()
    if self.huoqiu then
        GameplayHitTest(self.entity,1,TestSphereCollider(1),GetTransform(self.huoqiu),"OnHit")
    end
    
    self:StartHitTest()
end

function HuoQiuGAScript:OnHit(hits)
    for _,e in pairs(hits) do
        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= self.spec.source then
            ActorChangeState(unique_id)
            ApplyCueToTarget(unique_id,"Hurt")
            self:RemoveHuoqiu()
        end
    end
end

function HuoQiuGAScript:RemoveHuoqiu()
    if self.huoqiu then
        Destory(self.huoqiu)
    end

    self:EndAbility()
end

return HuoQiuGAScript