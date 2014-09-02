local Luua = require "Luua"
local Coroutine = require "Coroutine"
local Socket = require "Socket"

local coroutine = spawn(function(self)
    local socket = Socket(Socket.AF_INET, Socket.SOCK_STREAM, 0)
    socket:bind("0.0.0.0", 8888)
    socket:listen(10)
    while true do
        local client = socket:accept()
    end
end)

--spawn(function(self)
--    while true do
--        print("hello")
--        sleep(1)
--    end
--end)

coroutine:join()
