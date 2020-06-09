local milk = require("scripts.milk")
local gameplay = require("scripts.states.gameplay.gameplay")

local intro = {}

function intro.enter()
end

function intro.update(game)
    if milk.btnp(0) then
        game.pushState(gameplay)
    end
end

function intro.draw()
    milk.clrs()
    if milk.ticks % 64 < 48 then
        milk.sprfont(128 - 88 / 2, 100, "press start")
    end
end

return intro