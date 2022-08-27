local GAScript = require("GAS/GAScript")
local GA_JumpAScript = class("AttackGAScript",GAScript)

function GA_JumpAScript:ActiveAbility()
    self:CommitAbility()

    GameplayPlayAnimAndWait(self.entity,"Jump","AnimCompleted","AnimCanceled")
end

function GA_JumpAScript:AnimCompleted()    
    self:EndAbility()
end

function GA_JumpAScript:AnimCanceled()
    self:EndAbility()
end

return GA_JumpAScript