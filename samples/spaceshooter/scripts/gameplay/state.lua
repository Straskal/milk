local milk = require "scripts.milk"
local class = require "scripts.class"
local GameState = require "scripts.state"
local StarField = require "scripts.common.starfield"

local GameplayState = class("GameplayState", GameState)

function GameplayState:initialize()
    self.starField = StarField()
end

-- luacheck: push ignore self game
function GameplayState:enter(game)
    milk.loadspr("art/sprsheet.bmp")
    milk.play(0, 0, 128)
end
-- luacheck: pop

function GameplayState:update(_)
    self.starField:update()
end

function GameplayState:draw()
    self.starField:draw()
end

return GameplayState