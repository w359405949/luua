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

    self.hub.coroutines[self.coroutine] = self
end

function Coroutine:start()
    self.event = uv.uv_prepare_new(self.loop)
    uv.uv_prepare_start(self.event, self)
    self.stop_callback = uv.uv_prepare_stop
end

function Coroutine:notify_links()
    for i, co in ipairs(self.links) do
        co.resume()
    end
end

function Coroutine:_run()
    self.value = self:run()
    self:notify_links()
end

-- do what you want
function Coroutine:run()
end

function Coroutine:resume()
    self.event = uv.uv_prepare_new(self.loop)
    uv.uv_prepare_start(self.event, self)
    self.stop_callback = uv.uv_prepare_stop
end

function Coroutine:kill()
    if self:status() ~= "dead" then
        self.stop_callback(self.event)
    end
end

function Coroutine:join()
    if self:status() == "dead" then
        return 0, self.value
    elseif not self.hub:get_current() then
        self.hub:join()
        return 0, self.value
    else
        table.insert(self.links, self.hub:get_current())
        coroutine.yield()
        table.remove(self.links, self.hub:get_current())
        return 0, self.value
    end
end

function Coroutine:status()
    return coroutine.status(self.coroutine)
end

function Coroutine:wait(timeout)
    assert(self == self.hub:get_current(), "may block forever")
    self.event = uv.uv_timer_new(self.loop)
    uv.uv_timer_start(self.event, self, timeout, 0)
    self.stop_callback = uv.uv_timer_stop
    coroutine.yield()
end

return Coroutine
