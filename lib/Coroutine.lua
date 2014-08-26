local LCS = require "LCS"
local coroutine = require "coroutine"

Coroutine = LCS.class()

function Coroutine:init(hub, parent)
    self.parent = parent or hub
    self.hup = hub
    self.loop = hub.loop
    self.coroutine = coroutine.create(self._run)
    self.links = {}
    self.value = nil
end

function Coroutine:start()
    self.check = loop:uv_check()
    self.check:uv_check_start(self)
end

function Coroutine:_run()
    self.value = self.run()
    for i, co in ipairs(self.links) do
        uv_check_start(co.check, co)
    end
end

function Coroutine:run()
end

function Coroutine:kill()
    if coroutine.status(self.coroutine) ~= "dead" then
        self.check:uv_check_stop()
        self.coroutine = nil
    end
end

function Coroutine:join(co)
    if coroutine.status(self.coroutine) == "dead" then
        return 0, self.value
    elseif coroutine.status(self.coroutine) == "running" then
        table.insert(self.links, co)
        coroutine.yield()
        return 0, self.value
    end
end

return Coroutine
