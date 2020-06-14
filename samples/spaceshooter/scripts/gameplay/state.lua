local milk = require "scripts.milk"
local class = require "scripts.class"
local GameState = require "scripts.state"
local GameplayActionList = require "scripts.gameplay.actionList"
local PlayerShip = require "scripts.gameplay.playership"
local StarField = require "scripts.common.starfield"

local GameplayState = class("GameplayState", GameState)

function GameplayState:initialize()
    self.actionList = GameplayActionList(require("scripts.gameplay.level1.actions"))
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
    self.actionList:update()
end

function GameplayState:draw()
    self.starField:draw()
    self.actionList:draw()
end

return GameplayState