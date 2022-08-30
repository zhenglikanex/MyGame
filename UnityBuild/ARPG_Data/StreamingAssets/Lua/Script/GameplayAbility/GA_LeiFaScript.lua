local GAScript = require("GAS/GAScript")
local LeiFaGAScript = class("LeiFaGAScript",GAScript)

function LeiFaGAScript:ActiveAbility()
    --self:CommitAbility()
    self:CreateSphere()
    GameplayWait(self.entity,10,"RemoveSphere")
end

function LeiFaGAScript:CreateSphere()
    local spec = self:GetSpec()
    local transform = GetTransform(spec.source)
    local view = CreateView(spec.source,"Sphere",transform)
    if view then
        spec.entity_manager:add(1,GetLocal(view))
    end
end

function LeiFaGAScript:CheckHit()
    
end

function LeiFaGAScript:OnHit(hits)
    for k,v in pairs(hits) do
        print("CheckHit",v)
    end
end

function LeiFaGAScript:RemoveSphere()
    self:EndAbility()
end



return LeiFaGAScript