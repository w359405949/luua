require "Luua"

local Coroutine = require "Coroutine"
local Socket = require "Socket"

--local coroutine = spawn(function(self)
--    local socket = Socket(Socket.AF_INET, Socket.SOCK_STREAM, 0)
--    socket:bind("0.0.0.0", 8888)
--    socket:listen(10)
--    while true do
--        local client = socket:accept()
--        if client ~= nil then
--            spawn(function(self)
--                while true do
--                    local data = client:read(1)
--                    print("data: " .. data)
--                end
--            end)
--        end
--    end
--end)

--local coroutine = spawn(function(self)
--    while true do
--        print("hello")
--        sleep(1)
--    end
--end)
--

local coroutine = spawn(function(self)
    local socket = Socket(Socket.AF_INET, Socket.SOCK_STREAM, 0)
    local message = socket:connect("127.0.0.1", 8888)
    print(message)
    while true do
        socket:send("hello")
        sleep(1)
        print("hello")
    end
end)

coroutine:join()
