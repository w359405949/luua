local uv = require "libluv"
local LCS = require "LCS"

local Hub = LCS.class()

function Hub:init()
    self.parent = self
    self.loop = uv.uv_loop_new()
    self.coroutine = coroutine.create(self._run)
    self.coroutines = {__mode = "v"}
end

function Hub:_run()
    uv.uv_run(self.loop)
end

function Hub:join()
    local running, ismain = coroutine.running()
    assert(ismain, "may block forever")
    if coroutine.status(self.coroutine) == "dead" then
        return 0, self.value
    else
        self.value = coroutine.resume(self.coroutine, self)
        return 0, self.value
    end
end

function Hub:get_current()
    return self.coroutines[coroutine.running()]
end

return Hub
