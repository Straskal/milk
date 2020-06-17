local milk = require "scripts.milk"
local class = require "scripts.class"

local Enemies = class("Enemies")

function Enemies:initialize()
    self.enemies = {
        {xMove = math.random(256), yMove = -20, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true},
        {xMove = math.random(256), yMove = -20, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true},
        {xMove = math.random(256), yMove = -40, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true},
        {xMove = math.random(256), yMove = -80, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true},
        {xMove = math.random(256), yMove = -100, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true},
        {xMove = math.random(256), yMove = -120, x = 0, y = 0, sprite = 16, rotation = math.random(20), isAlive = true}
    }
end

function Enemies:getEnemies()
    local liveEnemies = {}
    for i = 1, #self.enemies do
        if self.enemies[i].isAlive then
            table.insert(liveEnemies, self.enemies[i])
        end
    end
    return liveEnemies
end

-- luacheck: push ignore self
function Enemies:destroy(enemy)
    if enemy.isAlive then
        enemy.yMove = -20
        enemy.xMove = math.random(256)
        enemy.rotation = math.random(20)
    end
end
-- luacheck: pop

local function checkOutOfBounds(enemy)
    return enemy.y > 224
end

function Enemies:update()
    for _, enemy in pairs(self:getEnemies()) do
        enemy.yMove = enemy.yMove + 1
        enemy.x = enemy.rotation * math.sin(milk.ticks / 20) + enemy.xMove
        enemy.y = enemy.rotation * math.cos(milk.ticks / 20) + enemy.yMove
        if checkOutOfBounds(enemy) then
            self:destroy(enemy)
        end
    end
end

function Enemies:draw()
    local enemies = self:getEnemies()
    for i = 1, #enemies do
        milk.spr(enemies[i].sprite, enemies[i].x - 8, enemies[i].y - 8)
    end
end

return Enemies
