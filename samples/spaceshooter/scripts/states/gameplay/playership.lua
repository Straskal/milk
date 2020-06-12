local milk = require("scripts.milk")

local btnup     = milk.btnup
local btndown   = milk.btndown
local btnleft   = milk.btnleft
local btnright  = milk.btnright
local btna      = milk.btna
local spr       = milk.spr

local function update(self)
    local mvx = 0
    local mvy = 0

    if milk.btn(btnup) then mvy = -self.speed end
    if milk.btn(btndown) then mvy = self.speed end
    if milk.btn(btnleft) then mvx = -self.speed end
    if milk.btn(btnright) then mvx = self.speed end
    if milk.btnp(btna) then milk.play(1, 1, 128) end

    self.xPos = self.xPos + mvx
    self.yPos = self.yPos + mvy

    if milk.ticks % 24 < 12 then self.sprite = 0
    else self.sprite = 1 end
end

local function draw(self)
    spr(self.sprite, self.xPos, self.yPos)
end

local function new()
    return {
        xPos 		= (256 / 2) - 16,
        yPos 		= (224 / 2) + 16,
        speed 		= 1,
        sprite 		= 0,
        update      = update,
        draw        = draw
    }
end

return {
    new = new
}
