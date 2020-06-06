local milk = require("scripts.milk")

local introState = {}

function introState.enter()
    milk.allocsnd(0, "sounds/music.wav")
end

function introState.update()
    if milk.btp(milk.btna) then milk.play(0, 0, 128) end
    if milk.btnp(milk.btnb) then milk.pause(0) end
    if milk.btnp(milk.btnx) then milk.resume(0) end
end

function introState.draw()
    milk.spr(0, 20, 20, 2, 1, 5, 1)
end

return introState