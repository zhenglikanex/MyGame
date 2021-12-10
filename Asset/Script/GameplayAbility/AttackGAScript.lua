local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    dump({ 1,2,3 })
    dump(self)
    self:CommitAbility()
    DelayTask(0.5,function()
        print("this is AttackGa")
        self:EndAbility()
    end)
end

return AttackGAScript