local M = {}

function M.New()
    local Attribute = {}
    Attribute["Health"] = require("Health").new()
    Attribute["MaxHealth"] = require("MaxHealth").new() -- 注意将属性的最大值需要单独设置一个字段
end

return M