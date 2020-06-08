local milk = require("scripts.milk")

local introState = {}

function introState.enter()
    milk.loadspr("sprsheet.bmp")
    milk.allocsnd(0, "sounds/music.wav")
end

function introState.update()
    if milk.btnp(milk.btna) then milk.play(0, 0, 128) end
    if milk.btnp(milk.btnb) then milk.pause(0) end
    if milk.btnp(milk.btnx) then milk.resume(0) end
end

function introState.draw()
    milk.spr(0, 0, 0, 16, 16, 1, 0)
end

return introState