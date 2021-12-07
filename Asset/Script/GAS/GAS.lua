function LoadGameplayEffect(name,ge)
    local template = require("GameplayEffect/".. name)
    CopyTable(template,ge)
end

function LoadGameplayAbility(name,ga)
    local template = require("GameplayAbility/" .. name)
    CopyTable(template,ga)
end
