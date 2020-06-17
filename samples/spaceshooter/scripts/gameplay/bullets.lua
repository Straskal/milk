local milk = require "scripts.milk"
local class = require "scripts.class"

local POOL_SIZE = 10

local BulletPool = class("BulletPool")

function BulletPool:initialize()
    self.pool = {}
    for i = 1, POOL_SIZE do
        self.pool[i] = {
            x = 0,
            y = 0,
            direction = 0,
            isActive = false
        }
    end
end

local function checkOutOfBounds(bullet)
    return bullet.x < 0 or bullet.x > 256 or bullet.y < 0 or bullet.y > 224
end

function BulletPool:checkCollision(target)
    for i = 1, POOL_SIZE do
        local bullet = self.pool[i]
        if
            bullet.isActive
            and bullet.x > target.x - 8
            and bullet.x < target.x + 8
            and bullet.y < target.y + 8
            and bullet.y > target.y - 8
        then
            bullet.isActive = false
            return true
        end
    end
    return false
end

function BulletPool:update()
    for i = 1, POOL_SIZE do
        local bullet = self.pool[i]
        if bullet.isActive then
            bullet.y = bullet.y + bullet.direction
            if checkOutOfBounds(bullet) then
                bullet.isActive = false
            end
        end
    end
end

function BulletPool:draw()
    for i = 1, POOL_SIZE do
        local bullet = self.pool[i]
        if bullet.isActive then
            milk.spr(2, bullet.x - 8, bullet.y - 3)
        end
    end
end

function BulletPool:create(x, y, direction)
    for i = 1, POOL_SIZE do
        local bullet = self.pool[i]
        if not bullet.isActive then
            bullet.isActive = true
            bullet.x = x + 8
            bullet.y = y + 2
            bullet.direction = direction
            break
        end
    end
end

return BulletPool
