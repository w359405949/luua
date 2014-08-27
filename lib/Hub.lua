local coroutine = require "coroutine"
local LCS = require "LCS"
local uv = require "uv"

Hub = LCS.class()

function Hub:init()
    self.parent = self
    self.loop = uv:new_loop()
    self.coroutine = coroutine.create(self.start)
end

function Hub:start()
    self.loop:uv_run()
end

function Hub:join()
    -- only main coroutine can join
    local running, ismain = coroutine.running()
    if coroutine.status(self.co) == "dead" then
        return 0, self.value
    elseif coroutine.status(self.coroutine) == "normal" then
        self.value = coroutine.resume(self.coroutine, self)
        return 0, self.value
    end
end

return Hub
