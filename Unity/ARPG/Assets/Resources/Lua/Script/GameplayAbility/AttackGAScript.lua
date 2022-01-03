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

    print("lua")
    print(self.spec)
    print(self.spec.source)

    PlayAnimation(self.spec.source,"skill100010",0)
end

function AttackGAScript:WaitAttack()
    print("this is niubi galasi")
end

return AttackGAScript