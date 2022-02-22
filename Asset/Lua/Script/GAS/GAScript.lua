local GAScript = class("GAScript")

function GAScript:ctor(entity)
    self.entity = entity
end

function GAScript:GetSpec()
    return GetGameplayAbilitySpec(self.entity)
end

function GAScript:GetLocal()
    return GetLocal(self.entity)
end

function GAScript:ActiveAbility()

end

function GAScript:CommitCost()
    CommitGameplayAbilityCost(self.entity)
end

function GAScript:CommitCooldown()
    CommitGameplayAbilityCooldown(self.entity)
end

function GAScript:CommitAbility()
    self:CommitCost()
    self:CommitCooldown()
end

function GAScript:EndAbility()
    local entity = self.entity
    Destory(entity)
end

return GAScript