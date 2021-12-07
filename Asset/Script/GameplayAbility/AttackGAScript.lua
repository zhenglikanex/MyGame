local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript")

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    DelayTask(0.5,function()
        print("this is AttackGa")
        self:EndAbility()
    end)
end

return AttackGAScript