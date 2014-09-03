local uv = require "libluv"
local LCS = require "LCS"
local Socket = LCS.class()

-- more supported
Socket.AF_INET = 2
Socket.AF_INET6 = 10
Socket.SOCK_STREAM = 1
Socket.SOCK_DGRAM = 2

function Socket:init(family, type, proto, sock)
    self._family = family
    self._type = type
    self._proto = proto
    self._socket = sock

    if family == Socket.AF_INET and type == Socket.SOCK_STREAM then
        self._socket = self._socket or uv.uv_tcp_new(get_hub():loop())
        self._bind = uv.uv_tcp_bind
        self._connect = uv.uv_tcp_connect
        self._listen = uv.uv_listen
        self._accept = uv.uv_accept
        self._read_start = uv.uv_read_start
        self._read_stop = uv.uv_read_stop
        self._write = uv.uv_write
    else
        assert(false, "only support family = Socket.AF_INET, type = Socket.SOCK_STREAM currently")
    end

    self._buf = ""
end

function Socket:bind(host, port, flags)
    return self._bind(self._socket, host, port, flags)
end

function Socket:connect(host, port)
    local result = self._connect(self._socket, get_current(), host, port)
    if result then
        return result
    end
    return coroutine.yield()
end

function Socket:listen(backlog)
    self._backlog = backlog
    return self._listen(self._socket, get_current(), self._backlog)
end

--TODO(ISSUE): never stopped
function Socket:accept()
    local peer = coroutine.yield() -- TODO: why return twice
    if peer == nil then
        return nil
    end

    assert(type(peer) == "userdata", "get unexpect data")
    return Socket(self._family, self._type, self._proto, peer)
end

function Socket:recv(len)

    self._read_start(self._socket, get_current())
    while string.len(self._buf) < len do
        local buf, message = coroutine.yield()
        if buf == nil  then
            return nil, message
        end
        assert(type(buf) == "string", "buf must be string")
        self._buf = self._buf .. buf
    end
    self._read_stop(self._socket)
    local result = string.sub(self._buf, 1, len)
    self._buf = string.sub(self._buf, len + 1)
    return result
end

function Socket:send(buf)
    self._write(self._socket, get_current(), buf)
    return coroutine.yield()
end

function Socket:close()
    self._close(self._socket, get_current())
    return coroutine.yield()
end

return Socket
