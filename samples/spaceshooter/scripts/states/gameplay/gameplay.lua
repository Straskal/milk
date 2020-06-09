local milk = require("scripts.milk")

local gameplay = {}

function gameplay.enter()
    milk.loadspr("art/sprsheet.bmp")
    milk.loadsnd(0, "sounds/music.wav")
    milk.play(0, 0, 128)
end

function gameplay.update()
end

function gameplay.draw()
end

return gameplay