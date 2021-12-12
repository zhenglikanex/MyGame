local GAScript = require("GAS/GAScript")
local AttackGAScript = class("AttackGAScript",GAScript)

function AttackGAScript:ActiveAbility()
    self:CommitAbility()
    DelayTask(self.entity,0.5,function()
        print("this is attack ga")
        self:EndAbility()
    end)

    GameplayEvent("tag",function()
        
    end)
end

return AttackGAScript