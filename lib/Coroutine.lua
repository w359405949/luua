local uv = require "libluv"
local LCS = require "LCS"

local Coroutine = LCS.class()

function Coroutine:init(hub, parent)
    assert(hub, "Coroutine init:hub can not be nil")
    self.parent = parent or hub
    self.hub = hub
    self.loop = hub.loop
    self.coroutine = coroutine.create(self._run)
    self.links = {}
    self.value = nil

    table.insert(self.hub.coroutines, self)
end

function Coroutine:start()
    self.check = uv.uv_check(self.loop)
    uv.uv_check_start(self.check, self)
end

function Coroutine:notify_links()
    for i, co in ipairs(self.links) do
        co.resume()
    end
end

function Coroutine:_run()
    self.value = self.run()
    self.notify_links()
end

function Coroutine:run()
    -- what you wnat to do
end

function Coroutine:resume()
    uv.uv_check_start(self.check, self)
end

function Coroutine:kill()
    if coroutine.status(self.coroutine) ~= "dead" then
        uv.uv_check_stop(self.check)
    end
end

function Coroutine:join()
    local main, ismain = coroutine.running()

    if coroutine.status(self.coroutine) == "dead" then
        return 0, self.value
    elseif ismain then
        self.hub.join()
        return 0, self.value
    else
        local co = self.hub:get_current()
        table.insert(self.links, co)
        coroutine.yield()
        return 0, self.value
    end
end

function Coroutine:wait(timeout)
    local main, ismain = coroutine.running()
    local timer = uv.uv_timer_new(self.hub.loop)
    uv.uv_timer_start(timer, self, timeout, 0)
    if ismain then
        self.hub:join()
    else
        coroutine.yield()
    end
end

return Coroutine
