local Luua = require "Luua"
local Coroutine = require "Coroutine"

local coroutine = Luua.spawn(function()
    while true do
        print("hello")
        Luua.sleep(1)
    end
end)

--local coroutine = Coroutine(Luua.get_hub())
--
--coroutine:start()
--coroutine:join()
Luua.sleep(1)
