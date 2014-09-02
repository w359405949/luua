local LCS = require "LCS"
local Hub = require "Hub"
local Coroutine = require "Coroutine"
local Luua = LCS.class()

hub = Hub()

function get_hub()
    return hub
end

function msleep(timeout)
    local coroutine = hub:get_current()
    assert(coroutine, "may block forever")
    coroutine:wait(timeout)
end

function sleep(timeout)
    msleep(timeout * 1000)
end

function spawn(run)
    local coroutine = Coroutine()
    coroutine.run = run
    coroutine:start()
    return coroutine
end

function get_current()
    return hub:get_current()
end
