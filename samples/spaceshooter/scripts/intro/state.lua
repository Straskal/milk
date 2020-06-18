local milk = require "scripts.milk"
local GameState = require "scripts.state"
local StarField = require "scripts.common.starfield"
local GameplayState = require "scripts.gameplay.state"

local TITLE = "STAR GAME II"
local SUBTITLE = "Return of The Bad Bois"
local PROMPT = "press Z"

local IntroState = GameState:subclass("IntroState")

function IntroState:initialize()
    self.starField = StarField()
end

--  luacheck: push ignore self game
function IntroState:enter(game)
    milk.loadsnd(0, "sounds/intro_music.wav")
    milk.play(0, 0, 128)
end

function IntroState:update(game)
    self.starField:update()

    if milk.btnp(5) then
        game.popState()
        game.pushState(GameplayState())
    end
end

local function printCenteredHorizontal(val, y, scale, color)
    local length = string.len(val)
    local halfLength = length / 2
    local halfResolution = 256 / 2
    local characterSize = 8 * scale

    milk.sprfont(halfResolution - halfLength * characterSize, y, val, scale, color)
end

local function drawTitle()
    printCenteredHorizontal(TITLE, 60, 2, 0x00ff00)
end

local function drawSubtitle()
    local subtitleLength = string.len(SUBTITLE)
    local halfResolution = 256 / 2
    local x = (halfResolution - (subtitleLength / 2) * 8) - 8

    for i = 1, subtitleLength do
        local character = SUBTITLE:sub(i, i)
        local adjustedTime = milk.ticks + i * 4
        local y = 85 + math.sin(adjustedTime / 15) * 4

        milk.sprfont(x + 8 * i, y, character, 1, 0x008751)
    end
end

local function drawPressStart()
    if milk.ticks % 64 < 48 then
        printCenteredHorizontal(PROMPT, 164, 1)
    end
end

function IntroState:draw(game)
    milk.clrs()
    self.starField:draw()
    drawTitle()
    drawSubtitle()
    drawPressStart()
end
-- luacheck: pop

return IntroState
