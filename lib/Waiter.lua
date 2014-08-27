local LCS = require "LCS"

Waiter = LCS.class()

function Waiter:init()
    self.coroutine = coroutine.running()
end

function Waiter:get()
    coroutine.yield()
end
