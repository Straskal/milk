local milk = require("scripts.milk")

local introState = {}

function introState.enter()
    milk.loadsnd(0, "sounds/music.wav")
end

function introState.update()
    if milk.btnp(milk.btna) then milk.plsnd(0, 0, 128) end
    if milk.btnp(milk.btnb) then milk.pssnd(0) end
    if milk.btnp(milk.btnx) then milk.rssnd(0) end
end

function introState.draw()
    milk.sprfont(10, 10, "this is the intro state.")
    milk.sprfont(20, 20, tostring(milk.slot(0)))
end

return introState