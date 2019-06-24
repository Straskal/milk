local graphics = require("milk.graphics")
local keyboard = require("milk.keyboard")
local animatedsprite = require("res.animatedsprite")
local flip_flags = graphics.flip_flags
local keys = keyboard.keys

-- constants
local PLAYER_SPEED = 2

-- module
local player = {}

-- mt
local Player_mt = {}

function Player_mt:init()
    self.x = 0
    self.y = 0
    self.animations = {
        idle = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
        walk = { 15, 16, 17, 18, 19, 20, 21 }
    }
    self.sprite = animatedsprite.new()
    self.sprite:init({
        texture_name = "res/player.png",
        initial_anim = self.animations.idle,
        rows = 4,
        columns = 8,
        seconds_per_frame = 0.1
    })
end

function Player_mt:tick()
    self.sprite:tick()

    self.vx = 0
    self.vy = 0

    if keyboard.is_key_down(keys.W) then
        self.vy = -1 * PLAYER_SPEED
    end
    if keyboard.is_key_down(keys.S) then 
        self.vy = 1 * PLAYER_SPEED
    end
    if keyboard.is_key_down(keys.A) then 
        self.vx = -1 * PLAYER_SPEED
        self.sprite:flip_x(true)
    end
    if keyboard.is_key_down(keys.D) then 
        self.vx = 1 * PLAYER_SPEED
        self.sprite:flip_x(false)
    end

    self.x = self.x + self.vx
    self.y = self.y + self.vy
    local anim = (((self.vx == 0 and self.vy == 0) and self.animations.idle) or self.animations.walk)
    self.sprite:set_animation(anim)
end

function Player_mt:draw()
    self.sprite:draw(self.x, self.y)
end

function player.new()
    local self = {}
    setmetatable(self, { __index = Player_mt })
    return self
end

return player