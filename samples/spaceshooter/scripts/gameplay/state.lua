local milk = require "scripts.milk"
local class = require "scripts.class"
local GameState = require "scripts.state"
local StarField = require "scripts.common.starfield"
local PlayerShip = require "scripts.gameplay.playership"

local GameplayState = class("GameplayState", GameState)

function GameplayState:initialize()
    self.starField = StarField()
    self.playerShip = PlayerShip()
end

-- luacheck: push ignore self game
function GameplayState:enter(game)
    milk.loadspr("art/sprsheet.bmp")
    milk.play(0, 0, 128)
    self.playerShip:load()
end
-- luacheck: pop

function GameplayState:update(_)
    self.starField:update()
    self.playerShip:update()
end

function GameplayState:draw()
    self.starField:draw()
    self.playerShip:draw()
end

return GameplayState