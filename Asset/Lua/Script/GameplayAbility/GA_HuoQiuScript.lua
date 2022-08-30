local GAScript = require("GAS/GAScript")
local HuoQiuGAScript = class("HuoQiuGAScript",GAScript)

function HuoQiuGAScript:ActiveAbility()
    self:CommitAbility()
    GameplayWait(self.entity,5,"EndAbility")
    self:CreateHuoqiue()
end

function HuoQiuGAScript:CreateHuoqiue()
    local spec = self:GetSpec()

    local huoqiu = CreateMovementView(spec.source,"HuoQiu",GetTransform(spec.source),vec3.new(0,0,1),vec3.new(0,0,1),vec3.new(0,0,0))
    if huoqiu then
        print("------------------------CreateHuoqiue1111111111")
        spec.entity_manager:add(1,GetLocal(huoqiu))
    end
end

function HuoQiuGAScript:StartHitTest()
    GameplayWait(self.entity,0,"OnHitTest")
end

function HuoQiuGAScript:OnHitTest()
    local spec = self:GetSpec()
    local huoqiu = spec.entity_manager:find(1)

    if huoqiu then
        GameplayHitTest(self.entity,1,TestSphereCollider(1),GetTransform(huoqiu),"OnHit")
    end
    
    self:StartHitTest()
end

function HuoQiuGAScript:OnHit(hits)
    local spec = self:GetSpec()

    for _,e in pairs(hits) do
        local unique_id = GetUniqueId(e)
        if unique_id and unique_id ~= spec.source then
            ApplyCueToTarget(e,"Hurt")
            self:EndAbility()
        end
    end
end

return HuoQiuGAScript