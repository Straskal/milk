local milk = require "scripts.milk"
local bullets = require "scripts.gameplay.bullets"

local btnup     = milk.btnup
local btndown   = milk.btndown
local btnleft   = milk.btnleft
local btnright  = milk.btnright
local btna      = milk.btna
local spr       = milk.spr

local function init(self)
    self = self or {}
    self.bulletPool = bullets.init()
    self.x = 0
    self.y = 0
    self.speed = 1
    self.sprite = 0
    return self
end

local function animate(self)
    if milk.ticks % 24 < 12 then
        self.sprite = 0
    else
        self.sprite = 1
    end
end

local function update(self)
    local mvx = 0
    local mvy = 0

    if milk.btn(btnup) then mvy = -self.speed end
    if milk.btn(btndown) then mvy = self.speed end
    if milk.btn(btnleft) then mvx = -self.speed end
    if milk.btn(btnright) then mvx = self.speed end

    if milk.btnp(btna) then
        milk.play(1, 1, 128)
        self.bulletPool.create(2, self.x, self.y, -1)
    end

    self.x = self.x + mvx
    self.y = self.y + mvy

    animate(self)
end

local function draw(self)
    spr(self.sprite, self.x, self.y)
end

return {
    init = init,
    update = update,
    draw = draw
}
