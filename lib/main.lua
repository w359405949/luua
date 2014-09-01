local Luua = require "Luua"
local Coroutine = require "Coroutine"

local coroutine = Luua.spawn(function(self)
    while true do
        print(self.message)
        Luua.sleep(1)
    end
end)
coroutine.message = "hello"

local coroutine2 = Luua.spawn(function(self)
    while true do
        print("world")
        Luua.sleep(0.5)
    end
end)


coroutine:join()
