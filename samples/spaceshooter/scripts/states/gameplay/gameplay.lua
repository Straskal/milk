local milk = require("scripts.milk")
local starfield = require("scripts.states.gameplay.starfield")
local starship = require("scripts.states.gameplay.playership")

local gameplay = {
    flyingIn = true,
    player = starship.new()
}

local function flyIn()
    if gameplay.flyingIn then
        gameplay.player.yPos = gameplay.player.yPos - 1
    end
    if (gameplay.player.yPos < 224 - 64) then
        gameplay.flyingIn = false
    end
    return gameplay.flyingIn
end

function gameplay.enter()
    milk.loadspr("art/sprsheet.bmp")
    milk.loadsnd(1, "sounds/fireball_shoot.wav")
    milk.play(0, 0, 128)
    gameplay.player.yPos = 224 + 32
    gameplay.player.xPos = 256 / 2 - 8
end

function gameplay.update()
    starfield.update()
    if not flyIn() then
        gameplay.player:update()
    end
end

function gameplay.draw()
    milk.clrs()
    starfield.draw()
    gameplay.player:draw()
end

return gameplay