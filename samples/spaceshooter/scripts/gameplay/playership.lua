local milk = require "scripts.milk"
local class = require "scripts.class"
local BulletPool = require "scripts.gameplay.bullets"

local btnup     = milk.btnup
local btndown   = milk.btndown
local btnleft   = milk.btnleft
local btnright  = milk.btnright
local btna      = milk.btna
local spr       = milk.spr

local ATTACK_TICK_BUFFER = 30

local PlayerShip = class("PlayerShip")

function PlayerShip:initialize()
    self.x = 0
    self.y = 0
    self.speed = 1
    self.sprite = 0
    self.bulletPool = BulletPool()
    self.bulletTimer = 0
end

local function attack(self)
    if milk.ticks > self.bulletTimer then
        milk.play(1, 1, 128)
        self.bulletPool:create(self.x, self.y, -3)
        self.bulletTimer = milk.ticks + ATTACK_TICK_BUFFER
    end
end

-- luacheck: push ignore self
function PlayerShip:load()
    milk.loadsnd(1, "sounds/fireball_shoot.wav")
end
-- luacheck: pop

function PlayerShip:update(_)
    local mvx = 0
    local mvy = 0

    if milk.btn(btnup) then mvy = -self.speed end
    if milk.btn(btndown) then mvy = self.speed end
    if milk.btn(btnleft) then mvx = -self.speed end
    if milk.btn(btnright) then mvx = self.speed end
    if milk.btn(btna) then attack(self) end

    self.x = self.x + mvx
    self.y = self.y + mvy
    self.bulletPool:update()
    self:_animate()
end

function PlayerShip:_animate()
    if milk.ticks % 24 < 12 then
        self.sprite = 0
    else
        self.sprite = 1
    end
end

function PlayerShip:draw()
    self.bulletPool:draw()
    spr(self.sprite, self.x, self.y)
end

return PlayerShip
