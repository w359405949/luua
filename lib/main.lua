local Luua = require "Luua"
local Coroutine = require "Coroutine"

local coroutine = spawn(function(self)
    local i = 1
    while true do
        print(i)
        i = i + 1
        sleep(0.0002)
    end
end)

local coroutine2 = spawn(function(self)
    local i = 1000
    while true do
        print(i)
        i = i + 1
        sleep(0.0001)
    end
end)


coroutine:join()
coroutine2:join()
