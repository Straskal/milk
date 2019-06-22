-- imports
local graphics = require("milk.graphics")
local keyboard = require("milk.keyboard")
local flip_flags = graphics.flip_flags
local keys = keyboard.keys

-- constants
local SECONDS_PER_ANIM_FRAME = 0.1
local PLAYER_SPEED = 2

local set_source_rect = function(self)
    local row = math.floor((self.current_anim[self.current_anim_frame] - 1) / self.anim_columns)
    local column = math.floor((self.current_anim[self.current_anim_frame] - 1) % self.anim_columns)
    self.srcx = column * self.anim_frame_size
    self.srcy = row * self.anim_frame_size
end

local set_animation = function(self, anim)
    if self.current_anim ~= anim then
        self.current_anim = anim
        self.current_anim_frame = 1
        set_source_rect(self)
    end
end

local init = function(self)
    self.texture = graphics.new_texture("res/player.png")
    self.x = 0
    self.y = 0
    self.vx = 0
    self.vy = 0
    self.srcx = 0
    self.srcy = 0
    self.srcw = 64
    self.srch = 64
    self.last_anim_time = 0
    self.flip = 0
    self.animations = {
        idle = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
        walk = { 15, 16, 17, 18, 19, 20, 21 }
    }
    self.anim_frame_size = 64
    self.anim_rows = 3
    self.anim_columns = 8
    self.current_anim_frame = 1
    self.current_anim = self.animations.idle
end

local tick = function(self)
    -- animation
    local totaltime = os.clock()
    if totaltime - self.last_anim_time > SECONDS_PER_ANIM_FRAME then
        self.current_anim_frame = self.current_anim_frame + 1
        if self.current_anim_frame > #self.current_anim then
            self.current_anim_frame = 1
        end
        set_source_rect(self)
		self.last_anim_time = totaltime
    end

    -- input
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
        self.flip = self.flip | flip_flags.X;
    end
    if keyboard.is_key_down(keys.D) then 
        self.vx = 1 * PLAYER_SPEED
        self.flip = self.flip & ~flip_flags.X;
    end
    self.x = self.x + self.vx
    self.y = self.y + self.vy
    if self.vx == 0 and self.vy == 0 then
        set_animation(self, self.animations.idle)
    else
        set_animation(self, self.animations.walk)
    end
end

local draw = function(self)
    graphics.set_draw_color(1, 1, 1, 1)
    graphics.drawx(self.texture, self.x, self.y, self.srcx, self.srcy, self.srcw, self.srch, 1, self.flip)
end

local new = function()
    return { 
        init = init,
        tick = tick,
        draw = draw
     }
end

return {
    new = new
}