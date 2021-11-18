package.path=package.path .. ";Script/?.lua"
require("Base/Base")

__G__TRACKBACK__ = function(msg)
    local msg = debug.traceback(msg, 3)
    print(msg)
    return msg
end

function f(obj)
    obj.period.periodic_inhibition_policy = 10
    print(obj.stacking.stack_limit_count)
end

function f3(num)
    print("this is f3");
    print(num)
end

function f2(obj)
    xpcall(function()
        local ge = GameplayEffect.new()
        print(ge.period.period)
        local application = GEApplication.new()
        print("key",application.chance_to_apply_to_target)
        print("this is f2")
        print(type(obj))
        print(obj)
        print("this is f2")
    end,__G__TRACKBACK__)
end

local function main()
    
end

xpcall(main,__G__TRACKBACK__)