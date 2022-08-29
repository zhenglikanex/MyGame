package.path=package.path ..";" .. load_path
require("Base/Base")
require("GAS/GAS")
require("GAS/GEDefine")
require("GAS/GameplayTagDefine")
require("GameplayAttribute")

__G__TRACKBACK__ = function(msg)
    local msg = debug.traceback(msg, 3)
    print(msg)
    return msg
end

local function main()
    
end

xpcall(main,__G__TRACKBACK__)