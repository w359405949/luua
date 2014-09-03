local uv = require "libluv"
local LCS = require "LCS"
local Socket = LCS.class()

-- more supported
Socket.AF_INET = 2
Socket.AF_INET6 = 10
Socket.SOCK_STREAM = 1
Socket.SOCK_DGRAM = 2

function Socket:init(family, type, proto, sock)
    self.family = family
    self.type = type
    self.proto = proto
    self.socket = sock

    if family == Socket.AF_INET and type == Socket.SOCK_STREAM then
        self.socket = self.socket or uv.uv_tcp_new(get_hub().loop)
        self._bind = uv.uv_tcp_bind
        self._connect = uv.uv_tcp_connect
        self._listen = uv.uv_listen
        self._accept = uv.uv_accept
        self._read = uv.uv_read_start
        self._write = uv.uv_write
    else
        assert(false, "only support family = Socket.AF_INET, type = Socket.SOCK_STREAM currently")
    end

    self.buf = ""
end

function Socket:bind(host, port, flags)
    return self._bind(self.socket, host, port, flags)
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

function Socket:listen(backlog)
    self.backlog = backlog
    return self._listen(self.socket, get_current(), self.backlog)
end

--TODO(ISSUE): never stopped, DDOS
function Socket:accept()
    local peer = coroutine.yield()
    if peer ~= nil then
        return Socket(self.family, self.SOCK_STREAM, self.proto, peer)
    end
    return nil
end

function Socket:read(len)
    while string.len(self.buf) < len do
        self._read(self.socket, get_current())
        local buf, message = coroutine.yield()
        if buf == nil then
            return buf, message
        end
        self.buf = self.buf .. buf
    end
    self.buf = string.sub(self.buf, len + 1)
    return string.sub(self.buf, 1, len)
end

function Socket:write(buf)
    self._write(buf)
end

function Socket:close()
    self._close(self.socket)
end

return Socket
