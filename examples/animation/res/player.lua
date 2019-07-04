local graphics = require("milk.graphics")
local keyboard = require("milk.keyboard")
local time = require("milk.time")
local animator = require("res.animator")
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
    self.image = graphics.new_image("res/player.png")
    self.flippedx = 1
    self.animations = {
        idle = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
        walk = { 15, 16, 17, 18, 19, 20, 21 }
    }
    self.animator = animator.new({
        frame_height = 64,
        frame_width = 64,
        rows = 4,
        columns = 8,
        seconds_per_frame = 0.1,
        initial_anim = self.animations.idle
    })
end

function Player_mt:tick()
	local dt = time.get_delta()

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
        self.flippedx = -1
    end
    if keyboard.is_key_down(keys.D) then 
        self.vx = 1 * PLAYER_SPEED
        self.flippedx = 1
    end

    self.x = self.x + self.vx
    self.y = self.y + self.vy
    local anim = (((self.vx == 0 and self.vy == 0) and self.animations.idle) or self.animations.walk)
    self.animator:set_animation(anim)
end

function Player_mt:draw()
    local sx, sy, sw, sh = self.animator:tick()
    graphics.drawx(self.image, self.x - 32, self.y - 32, sx, sy, sw, sh, 1 * self.flippedx, 1, 0)
end

function player.new()
    local self = {}
    setmetatable(self, { __index = Player_mt })
    self:init()
    return self
end

return player