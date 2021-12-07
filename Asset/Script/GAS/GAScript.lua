local GAScript = class("GAScript")

function GAScript:ctor(entity,ga)
    self.entity = entity
    self.ga = ga
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
    EndAbility(self.entity)
end