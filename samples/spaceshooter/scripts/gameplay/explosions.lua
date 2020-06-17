local milk = require "scripts.milk"
local class = require "scripts.class"

local ANIM_START_SPRITE = 18
local NUM_FRAMES = 4
local ANIM_TIMER = 3
local POOL_SIZE = 10

local ExplosionPool = class("ExplosionPool")

function ExplosionPool:initialize()
    milk.loadsnd(2, "sounds/explode.wav")
    self.pool = {}
    for i = 1, POOL_SIZE do
        self.pool[i] = {
            x = 0,
            y = 0,
            frame = 0,
            timer = 0,
            isActive = false
        }
    end
end

function ExplosionPool:update()
    for i = 1, POOL_SIZE do
        local explosion = self.pool[i]
        if explosion.isActive and milk.ticks > explosion.timer then
            explosion.frame = explosion.frame + 1
            explosion.timer = milk.ticks + ANIM_TIMER
            if explosion.frame > ANIM_START_SPRITE + NUM_FRAMES then
                explosion.isActive = false
            end
        end
    end
end

function ExplosionPool:draw()
    for i = 1, POOL_SIZE do
        local explosion = self.pool[i]
        if explosion.isActive then
            milk.spr(explosion.frame, explosion.x - 8, explosion.y - 8)
        end
    end
end

function ExplosionPool:create(x, y)
    for i = 1, POOL_SIZE do
        local explosion = self.pool[i]
        if not explosion.isActive then
            explosion.isActive = true
            explosion.x = x
            explosion.y = y
            explosion.frame = ANIM_START_SPRITE
            explosion.timer = milk.ticks + ANIM_TIMER
            break
        end
    end
    milk.play(2, 2, 10)
end

return ExplosionPool
