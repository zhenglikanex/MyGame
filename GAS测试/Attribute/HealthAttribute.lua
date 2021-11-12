local M = {}

function M.New()
    local Attribute = {}
    Attribute.value = 0  --相当于持久化的改变
    return Attribute
end

return M