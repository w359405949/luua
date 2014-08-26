local Hub = require "Hub"
local Coroutine = require "Coroutine"
local Waiter = require "Waiter"
local LCS = require "LCS"

Luua = LCS.class()

function Luua:init()
    self.hub = Hub()
end

function Luua:new_coroutine(parent)
    local coroutine = Coroutine:new(self.hub, parent)
    return coroutine
end

function Luua:sleep(timeout)
    local waiter = Waiter:new()
    local timer = uv_timer(hub.loop)
    uv_timer_start(timer, waiter, timeout, 0)
    watier.get()
end

return Luua
