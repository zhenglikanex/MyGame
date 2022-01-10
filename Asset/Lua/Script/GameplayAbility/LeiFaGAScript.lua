local GAScript = require("GAS/GAScript")
local LeiFaGAScript = class("LeiFaGAScript",GAScript)

function LeiFaGAScript:ActiveAbility()
    self:CommitAbility()
       
    self:CreateSphere()
    GameplayWait(self.entity,5,"RemoveSphere")
end

function LeiFaGAScript:CreateSphere()
    local transform = GetTransform(self.spec.source)
    self.sphere = CreateView(self.spec.source,"Sphere",transform)
    self:CheckHit()
end

function LeiFaGAScript:CheckHit()
    if self.sphere then
        local transform = GetTransform(self.sphere)
        local radius = 2.5
        local hits = CheckSphereOverlap(transform,radius)
        for k,v in pairs(hits) do
            print("CheckHit",v)
        end
    end

    --GameplayWait(self.entity,1,"CheckHit")
end

function LeiFaGAScript:RemoveSphere()
    print("function LeiFaGAScript:RemoveSphere()",self.sphere)
    if self.sphere then
        Destory(self.sphere)
    end
    self:EndAbility()
end



return LeiFaGAScript