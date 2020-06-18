local milk = require "scripts.milk"
local BulletPool = require "scripts.gameplay.bullets"
local Enemies = require "scripts.gameplay.enemies"
local ExplosionPool = require "scripts.gameplay.explosions"
local GameState = require "scripts.state"
local PlayerShip = require "scripts.gameplay.playership"
local StarField = require "scripts.common.starfield"

local GameplayState = GameState:subclass("GameplayState")

function GameplayState:initialize()
    self.starField = StarField()
    self.player = PlayerShip()
    self.enemies = Enemies()
    self.bulletPool = BulletPool()
    self.explosions = ExplosionPool()
    self.player.x = 256 / 2
    self.player.y = 224 / 2
    self.score = 0
end

-- luacheck: push ignore self game
function GameplayState:enter(game)
    milk.loadspr("art/sprsheet.bmp")
    milk.loadsnd(0, "sounds/music.wav")
    milk.play(0, 0, 128)
    self.player:load()
end
-- luacheck: pop

local function checkPlayerBulletCollisions(self)
    local liveEnemies = self.enemies:getEnemies()
    for i = 1, #liveEnemies do
        if self.bulletPool:checkCollision(liveEnemies[i]) then
            self.enemies:destroy(liveEnemies[i])
            self.explosions:create(liveEnemies[i].x, liveEnemies[i].y)
            self.score = self.score + 1
        end
    end
end

local function checkPlayerEnemyCollision(self)
    local liveEnemies = self.enemies:getEnemies()
    for i = 1, #liveEnemies do
        if self.player:checkCollision(liveEnemies[i]) then
            self.enemies:destroy(liveEnemies[i])
            self.explosions:create(self.player.x, self.player.y)
            self.player:damage()
        end
    end
end

local function drawHearts(self)
    for i = 1, self.player.hearts do
        local xPos = 180 + i * 16
        milk.spr(3, xPos, 5)
    end
end

local function drawScore(self)
    milk.sprfont(10, 10, "score:" .. self.score)
end

function GameplayState:update()
    self.starField:update()
    self.enemies:update()
    self.player:update(self)
    self.bulletPool:update()
    self.explosions:update()

    checkPlayerBulletCollisions(self)
    checkPlayerEnemyCollision(self)
end

function GameplayState:draw(_)
    milk.clrs()

    self.starField:draw()
    self.bulletPool:draw()
    self.explosions:draw()
    self.enemies:draw()
    self.player:draw()

    drawHearts(self)
    drawScore(self)
end

return GameplayState
