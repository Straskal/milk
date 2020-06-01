local milk = require("scripts.milk")

local introState = {}

function introState.enter()
    milk.loadsnd(0, "sounds/music.wav")
    milk.snd(0, 128, true)
end

function introState.update()
end

function introState.draw()
    milk.sprfont(10, 10, "this is the intro state.")
end

return introState