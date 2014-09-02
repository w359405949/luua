local uv = require "libluv"
local LCS = require "LCS"
local Socket = LCS.class()

-- more supported
Socket.AF_INET = 2
Socket.AF_INET6 = 10
Socket.SOCK_STREAM = 1
Socket.SOCK_DGRAM = 2

function Socket:init( family, type, proto)
    if family == Socket.AF_INET and type == Socket.SOCK_STREAM then
        self.socket = uv.uv_tcp_new(get_hub().loop)
    end
end

function Socket:connect()
end

function Socket:listen()
end

function Socket:accept()
end

function Socket:read()
end

function Socket:write()
end

function Socket:close()
end

return Socket
