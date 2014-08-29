local LCS = require "LCS"
local Hub = require "Hub"
local Coroutine = require "Coroutine"
local Luua = LCS.class()

hub = Hub()

function Luua.get_hub()
    return hub
end

function Luua.msleep(timeout)
    local main, ismain = coroutine.running()
    if ismain then
        co = Coroutine(hub)
        co:wait(timeout)
    else
        co = hub:get_current()
        co:wait(timeout)
    end
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
