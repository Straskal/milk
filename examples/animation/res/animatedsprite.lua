local graphics = require("milk.graphics")
local flip_flags = graphics.flip_flags

-- private module helpers
local function set_source_rect(sprite)
    local row = math.floor((sprite.current_anim[sprite.current_anim_frame] - 1) / sprite.columns)
    local column = math.floor((sprite.current_anim[sprite.current_anim_frame] - 1) % sprite.columns)
    sprite.src_x = column * sprite.frame_width
    sprite.src_y = row * sprite.frame_height
end

-- module
local animatedsprite = {}

-- mt
local AnimatedSprite_mt = {}

function AnimatedSprite_mt:init(info)
    self.texture = graphics.new_texture(info.texture_name)
    self.animations = info.animations
    self.rows = info.rows
    self.columns = info.columns
    self.seconds_per_frame = info.seconds_per_frame or 0.1
    self.current_anim = info.initial_anim

    local w, h = self.texture:get_size()
    assert(w % self.columns == 0)
    assert(h % self.rows == 0)

    self.flip = flip_flags.NONE
    self.color = { 1, 1, 1, 1 }
    self.frame_width = w / self.columns
    self.frame_height = h / self.rows
    self.last_anim_time = 0
    self.current_anim_frame = 1
    set_source_rect(self)
end

function AnimatedSprite_mt:set_animation(animation)
    if self.current_anim ~= animation then
        self.current_anim = animation
        self.current_anim_frame = 1
        set_source_rect(self)
    end
end

function AnimatedSprite_mt:set_color(r, g, b, a)
    self.color[1] = r
    self.color[2] = g
    self.color[3] = b
    self.color[4] = a
end

function AnimatedSprite_mt:flip_x(toggle)
    if toggle then 
        self.flip = self.flip | flip_flags.X
    else
        self.flip = self.flip & ~flip_flags.X
    end
end

function AnimatedSprite_mt:flip_y(toggle)
    if toggle then 
        self.flip = self.flip | flip_flags.Y
    else
        self.flip = self.flip & ~flip_flags.Y
    end
end

function AnimatedSprite_mt:tick()
    local t = os.clock()
    if t - self.last_anim_time > self.seconds_per_frame then
        self.current_anim_frame = self.current_anim_frame + 1
        if self.current_anim_frame > #self.current_anim then
            self.current_anim_frame = 1
        end
        set_source_rect(self)
        self.last_anim_time = t
    end
end

function AnimatedSprite_mt:draw(x, y)
    graphics.set_draw_color(table.unpack(self.color))
    graphics.drawx(self.texture, x, y, self.src_x, self.src_y, self.frame_width, self.frame_height, 1, self.flip)
end

function animatedsprite.new()
    local self = {}
    setmetatable(self, { __index = AnimatedSprite_mt })
    return self
end

return animatedsprite