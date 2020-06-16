local milk = require "scripts.milk"
local class = require "scripts.class"
local GameState = require "scripts.state"
local StarField = require "scripts.common.starfield"
local GameplayState = require "scripts.gameplay.state"

local TITLE = "STAR GAME:"
local SUBTITLE = "Return of The Bad Bois"

local IntroState = class("IntroState", GameState)

function IntroState:initialize()
    self.starField = StarField()
end

--  luacheck: push ignore self game
function IntroState:enter(game)
    milk.loadsnd(0, "sounds/music.wav")
    milk.play(0, 0, 128)
end

function IntroState:update(game)
    self.starField:update()

    if milk.btnp(0) then
        game.popState()
        game.pushState(GameplayState())
    end
end

local function drawTitle()
    milk.sprfont(128 - string.len(TITLE) * 8 * 2 / 2, 60, TITLE, 2, 0x00ff00)
    milk.sprfont(128 - string.len(SUBTITLE) * 8 / 2, 60 + 8 * 2.5, SUBTITLE, 1, 0x00ff00)
end

local function drawPressStart()
    if milk.ticks % 64 < 48 then
        milk.sprfont(128 - 88 / 2, 164, "press start")
    end
end

function IntroState:draw(game)
    milk.clrs()
    self.starField:draw()

    milk.line(10, 10, 50, 50, 0xff0000)

    drawTitle()
    drawPressStart()
end
-- luacheck: pop

return IntroState
