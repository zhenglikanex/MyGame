local GAScript = require("GAS/GAScript")
local GA_DoubleJumpAScript = class("AttackGAScript",GAScript)

function GA_DoubleJumpAScript:ActiveAbility()
    self:CommitAbility()

    GameplayPlayAnimAndWait(self.entity,"Jump","AnimCompleted","AnimCanceled")
end

function GA_DoubleJumpAScript:AnimCompleted()    
    self:EndAbility()
end

function GA_DoubleJumpAScript:AnimCanceled()
    self:EndAbility()
end

return GA_DoubleJumpAScript