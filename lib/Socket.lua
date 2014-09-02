local uv = require "libluv"
local LCS = require "LCS"
local Socket = LCS.class()

-- more supported
Socket.AF_INET = 2
Socket.AF_INET6 = 10
Socket.SOCK_STREAM = 1
Socket.SOCK_DGRAM = 2

function Socket:init(family, type, proto)
    if family == Socket.AF_INET and type == Socket.SOCK_STREAM then
        self.socket = uv.uv_tcp_new(get_hub().loop)
        self._connect = uv.uv_tcp_connect
        self._listen = uv.uv_listen
        self._accept = uv.uv_accept
        self._read = uv.uv_read_start
        self._write = uv.uv_write
    end

    self.buf = ""
    self.closed = false
end

function Socket:connect()
    if self.closed then
        return
    end
    local result = self._connect(self.socket, get_current())
    if result then
        result = coroutine.yield()
    end
    return result
end

function Socket:listen()
    self._listen(self.socket, get_current())
end

function Socket:accept()
    local coroutine.yield()
    self._accept(self.socket, get_current())
end

function Socket:read(len)
    while string.len(self.buf) < len do
        self._read(self.socket, get_current())
        local buf, message = coroutine.yield()
        if buf == nil then
            return buf, message
        end
        self.buf = self.buf + buf
    end
    local buf = string.sub(self.buf, 1, len)
    self.buf = string.sub(self.buf, len + 1, string.len(self.buf))
    return buf
end

function Socket:write(buf)
    self._write(buf)
end

function Socket:close()
    self._close(self.socket)
end

return Socket
