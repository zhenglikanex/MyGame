local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    --ReceiveGameplayEvent(self.entity,"GA.AttackGA",function(target)
        --print("this is ",target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --ApplyGameplayEffectToTarget(self.entity,"AttackGE",1,target)
        --self:EndAbility()
    --end)
    GameplayPlayAnimAndWait(self.entity,"skill100010","AnimCompleted","AnimCanceled")
end

function AttackGAScript:AnimCompleted()
    print("this is AnimCompleted")
end

function AttackGAScript:AnimCanceled()
    print("this is AnimCanceled")
end

function AttackGAScript:WaitAttack()
    print("this is niubi galasi")
end

return AttackGAScript