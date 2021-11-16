print("hello sol!")

function f(obj)
    for k,v in pairs(obj.value) do
        print(k,v)
    end
end

return 10