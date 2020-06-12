local milk = require("scripts.milk")
local gameplay = require("scripts.states.gameplay.gameplay")
local starField = require("scripts.states.gameplay.starfield")

local TITLE = "STAR GAME:"
local TITLE2 = "Return of The Bad Bois"

local intro = {}

function intro.enter()
    milk.loadsnd(0, "sounds/music.wav")
    milk.play(0, 0, 128)
    starField.init()
end

function intro.update(game)
    starField.update()
    if milk.btnp(0) then
        game.popState()
        game.pushState(gameplay)
    end
end

function intro.draw()
    milk.clrs()
    starField.draw()

    milk.sprfont(128 - string.len(TITLE) * 8 * 2 / 2, 60, TITLE, 2, 0x00ff00)
    milk.sprfont(128 - string.len(TITLE2) * 8 / 2, 60 + 8 * 2.5, TITLE2, 1, 0x00ff00)

    if milk.ticks % 64 < 48 then
        milk.sprfont(128 - 88 / 2, 164, "press start")
    end
end

return intro