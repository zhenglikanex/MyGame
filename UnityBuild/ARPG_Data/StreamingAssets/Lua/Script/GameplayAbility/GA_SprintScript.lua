local GAScript = require("GAS/GAScript")
local GA_SprintScript = class("AttackGAScript",GAScript)

function GA_SprintScript:ActiveAbility()
    self:CommitAbility()

    GameplayInput(self.entity,"GA_Sprint","OnPressed","OnReleased")
end

function GA_SprintScript:OnPressed()    
    
end

function GA_SprintScript:OnReleased()
    self:EndAbility()
end

return GA_SprintScript