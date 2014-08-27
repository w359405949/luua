local classA = require "classA"
local LCS = require "LCS"

classB = LCS.class()

function classB:init()
    self.a = classA()
end

return classB
