local uv = require "libluv"
local LCS = require "LCS"
local Coroutine = LCS.class()

function Coroutine:init(hub)
    assert(hub, "Coroutine init:hub can not be nil")
    self.parent = hub:get_current() or hub
    self.hub = hub
    self.loop = hub.loop
    self.coroutine = coroutine.create(self._run)
    self.links = {}
    self.value = nil

    table.insert(self.hub.coroutines, self)
end

function Coroutine:start()
    self.event = uv.uv_check_new(self.loop)
    uv.uv_check_start(self.event, self)
end

function Coroutine:notify_links()
    for i, co in ipairs(self.links) do
        co.resume()
    end
end

function Coroutine:_run()
    print("??")
    self.value = self:run()
    self.notify_links()
end

function Coroutine:run()
    print("!!")
    assert(false, "no thing to do")
    -- what you wnat to do
end

function Coroutine:resume()
    uv.uv_check_start(self.event, self)
end

function Coroutine:kill()
    if self:status() ~= "dead" then
        uv.uv_check_stop(self.event)
    end
end

function Coroutine:join()
    local running, ismain = coroutine.running()

    if self:status() == "dead" then
        return 0, self.value
    elseif ismain then
        self.hub:join()
        return 0, self.value
    else
        local coroutine = self.hub:get_current()
        table.insert(self.links, coroutine)
        coroutine.yield()
        table.remove(self.links, coroutine)
        return 0, self.value
    end
end

function Coroutine:status()
    return coroutine.status(self.coroutine)
end

function Coroutine:wait(timeout)
    local running, ismain = coroutine.running()
    local timer = uv.uv_timer_new(self.hub.loop)
    uv.uv_timer_start(timer, self, timeout, 0)
    if ismain then
        self.hub:join()
    else
        coroutine.yield()
    end
end

return Coroutine
