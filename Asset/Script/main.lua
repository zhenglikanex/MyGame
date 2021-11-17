package.path=package.path .. ";Script/?.lua"
require("Base/Base")

__G__TRACKBACK__ = function(msg)
    local msg = debug.traceback(msg, 3)
    print(msg)
    return msg
end

function f(obj)
    for k,v in pairs(obj.value) do
        print(k,v)
    end
end

function f2(obj)
    print("f2!!!!")
    obj.value:add({ 1,3,4,5 })
    obj.value:add({ 1,3 })
    for _,tab in pairs(obj.value) do
        print("test")
    end

    print(#obj.value)
end

function f3(obj)
    print("f3!!!!")
    for _,tab in pairs(obj.value) do
        print("test")
    end
end

function f4(obj)
    xpcall(function()
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")
        obj.self:add("123")

        local t1 = os.clock()
        for i = 1,100000 do
            for k,v in pairs(obj.self) do
                
            end
        end

        print(os.clock() - t1)

        local tab = { 1,6,4,3,5,5,6,3,233,3,4,5,21,3,5,3,3,1,2,3,4,5,6,6,5 }
        print(#tab)
        local t1 = os.clock()
        for i = 1,100000 do
            for k,v in pairs(tab) do
                
            end
        end

        print(os.clock() - t1)

        obj:ApplyGETags("test")
        obj.ge_tags["test"]:add("123")
        
        print(obj,obj.self)
        dump(obj.self)
        dump(obj.ge_tags)
    end,__G__TRACKBACK__)
end

local function main()
    a = b + 1
    print("this is main")
end

xpcall(main,__G__TRACKBACK__)