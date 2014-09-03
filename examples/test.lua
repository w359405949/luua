local co = coroutine.create(function()
    local i = 1000
    while true do
        local result = coroutine.yield(i)
        print(result)
        --i = i + 1
    end
end)

local i = 1
while true do
    local i, j = coroutine.resume(co, "fuck")
    --print(j)
end
