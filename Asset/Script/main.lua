package.path=package.path .. ";Script/?.lua"
require("Base/Base")
require("GAS/GAS")
require("GAS/GEDefine")
require("GameplayAttribute")

__G__TRACKBACK__ = function(msg)
    local msg = debug.traceback(msg, 3)
    print(msg)
    return msg
end

local function main()
    
end

xpcall(main,__G__TRACKBACK__)