local LCS = require "LCS"
local coroutine = require "coroutine"
local uv = require "libluv"

classA = LCS.class()

function classA:init()
    self.loop = uv.uv_loop_new()
    self.coroutine = coroutine.create(self.run)
end

function classA:run()
end

return classA
