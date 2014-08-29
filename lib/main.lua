local Luua = require "Luua"
local Coroutine = require "Coroutine"

local coroutine = Luua.spawn(function(self)
    while true do
        print("hello")
        Luua.sleep(1)
    end
end)

coroutine:join()
