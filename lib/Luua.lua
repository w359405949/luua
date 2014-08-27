local LCS = require "LCS"
local Hub = require "Hub"
local Coroutine = require "Coroutine"

local Luua = LCS.class()

function Luua:init()
    self.hub = Hub()
end

function Luua:msleep(timeout)
    local main, ismain = coroutine.running()
    if ismain then
        co = Coroutine(self.hub)
        co:wait(timeout)
    else
        co = self.hub:get_current()
        co:wait(timeout)
    end
end

function Luua:sleep(timeout)
    self:msleep(timeout * 1000)
end

return Luua
