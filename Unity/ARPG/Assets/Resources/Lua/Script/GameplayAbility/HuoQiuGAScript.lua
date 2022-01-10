local GAScript = require("GAS/GAScript")
local HuoQiuGAScript = class("HuoQiuGAScript",GAScript)

function HuoQiuGAScript:ActiveAbility()
    self:CommitAbility()
    self:CreateHuoqiue()
end

function HuoQiuGAScript:CreateHuoqiue()
    self.huoqiu = CreateMovementColliderView(self.spec.source,self.loc,"HuoQiu",GetTransform(self.spec.source),vec3.new(0,0,1),vec3.new(0,0,1),vec3.new(0,0,0))
end

function HuoQiuGAScript:OnTrigger(hit)
    print("function HuoQiuGAScript:OnTrigger(hit)")
    local unique_id = GetUniqueId(hit)
    if unique_id and unique_id ~= self.spec.source then
       ActorChangeState(unique_id)
       ApplyCueToTarget(unique_id,"Hurt")
       self:RemoveHuoqiu()
    end
end

function HuoQiuGAScript:RemoveHuoqiu()
    if self.huoqiu then
        Destory(self.huoqiu)
    end

    self:EndAbility()
end

return HuoQiuGAScript