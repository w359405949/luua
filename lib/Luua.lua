local LCS = require "LCS"
local Hub = require "Hub"
local Coroutine = require "Coroutine"
local Luua = LCS.class()

hub = Hub()

function Luua.get_hub()
    return hub
end

function Luua.msleep(timeout)
    local coroutine = hub:get_current()
    assert(coroutine, "may block forever")
    coroutine:wait(timeout)
end

function Luua.sleep(timeout)
    Luua.msleep(timeout * 1000)
end

function Luua.spawn(run)
    local coroutine = Coroutine(hub)
    coroutine.run = run
    coroutine:start()
    return coroutine
end

return Luua
