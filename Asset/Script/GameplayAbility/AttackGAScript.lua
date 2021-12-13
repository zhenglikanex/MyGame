local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    ReceiveGameplayEvent(self.entity,"GA.AttackGA",function(target)
        print("this is ",target)
        ApplyGameplayEffectToTarget(target)
        self:EndAbility()
    end)
end

return AttackGAScript