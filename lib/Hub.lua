local uv = require "libluv"
local LCS = require "LCS"
local Hub = LCS.class()

function Hub:init()
    self._parent = self
    self._loop = uv.uv_loop_new()
    self._coroutine = coroutine.create(self._run)
    self._coroutines = {__mode = "v"}
end

function Hub:_run()
    assert(self._coroutine == coroutine.running(), "do not call _run directly")
    uv.uv_run(self._loop)
end

function Hub:join()
    assert(not self:get_current(), "may block forever")
    if self:status() == "dead" then
        return 0, self._value
    else
        self._value = coroutine.resume(self._coroutine, self)
        return 0, self._value
    end
end

function Hub:status()
    return coroutine.status(self._coroutine)
end

function Hub:get_current()
    return self._coroutines[coroutine.running()]
end

function Hub:loop()
    return self._loop
end

return Hub
