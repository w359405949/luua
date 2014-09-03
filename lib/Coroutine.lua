local uv = require "libluv"
local LCS = require "LCS"
local Coroutine = LCS.class()

function Coroutine:init()
    self._parent = get_current() or get_hub()
    self._hub = get_hub()
    self._loop = get_hub():loop()
    self._coroutine = coroutine.create(self._run)
    self._links = {}
    self._value = nil

    self._hub._coroutines[self._coroutine] = self
end

function Coroutine:start()
    assert(self ~= get_current(), "may block forever")
    self._event = uv.uv_prepare_new(self._loop)
    uv.uv_prepare_start(self._event, self)
    self._stop_callback = uv.uv_prepare_stop
end

function Coroutine:notify_links()
    for i, co in ipairs(self._links) do
        co.resume()
    end
end

function Coroutine:_run()
    assert(self._coroutine == coroutine.running(), "do not call _run directly")
    self._value = self:run()
    self:notify_links()
end

-- do what you want
function Coroutine:run()
end

function Coroutine:kill()
    if self:status() ~= "dead" then
        self._stop_callback(self._event)
    end
end

function Coroutine:join()
    assert(self ~= get_current(), "may block forever")
    if self:status() == "dead" then
        return 0, self._value
    elseif not get_current() then
        self._hub:join()
        return 0, self._value
    else
        table.insert(self._links, get_current())
        coroutine.yield()
        table.remove(self._links, get_current())
        return 0, self._value
    end
end

function Coroutine:status()
    return coroutine.status(self._coroutine)
end

function Coroutine:wait(timeout)
    assert(self == get_current(), "may block forever")
    self._stop_callback(self._event)
    self._event = uv.uv_timer_new(self._loop)
    uv.uv_timer_start(self._event, self, timeout, 0)
    self._stop_callback = uv.uv_timer_stop
    coroutine.yield()
end

return Coroutine
