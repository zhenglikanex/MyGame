print(load_path)
package.path=package.path ..";" .. load_path
print("package.path",package.path)
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