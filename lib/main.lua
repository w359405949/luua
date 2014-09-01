local Luua = require "Luua"
local Coroutine = require "Coroutine"

local coroutine = Luua.spawn(function(self)
    self.i = 1
    while true do
        --print(self.i)
        Luua.sleep(0.2)
    end
end)

local coroutine2 = Luua.spawn(function(self)
    self.i = 1000
    while true do
        --print(self.i)
        Luua.sleep(0.05)
    end
end)


coroutine:join()
coroutine2:join()
