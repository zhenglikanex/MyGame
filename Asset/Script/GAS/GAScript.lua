local GAScript = class("GAScript")

function GAScript:ctor()
end

function GAScript:Init(entity,spec)
    self.entity = entity
    self.spec = spec
end 

function GAScript:ActiveAbility()

end

function GAScript:CommitCost()
    
end

function GAScript:CommitCooldown()
    
end

function GAScript:CommitAbility()
    self:CommitCost()
    self:CommitCooldown()
end

function GAScript:EndAbility()
    local entity = self.entity
    self.entity = nil
    self.spec = nil

    DestoryEntity(entity)
end

return GAScript