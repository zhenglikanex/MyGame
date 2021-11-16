print("hello sol!")

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

return 10