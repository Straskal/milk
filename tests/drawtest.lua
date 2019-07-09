local window = require("milk.window")
local graphics = require("milk.graphics")
local keyboard = require("milk.keyboard")
local mainloop = require("tests.mainloop")
local keys = keyboard.keys

--[[
    the draw test proves the following:
    - the window icon is set properly
    - images are properly loaded
    - images are properly drawn to the screen
    - images are properly rotated and scaled
    - images are drawn with he proper draw color
--]]
local test = {}

function test:start()
    window.set_size(1280, 720)
    window.set_icon("tests/assets/milkicon.png")
    window.set_fullscreen(true)
    graphics.set_resolution(640, 360)

    self.image = graphics.new_image("tests/assets/milk.png")
    self.w, self.h = self.image:get_size()
    self.rotation = 0
    self.scale = 1
    self.scalar = 0.5
end

function test:tick(dt)
    if keyboard.is_key_released(keys.ESCAPE) then
        window.close()
    end
    self.rotation = self.rotation + (50 * dt)
    self.scale = self.scale + (self.scalar * dt)
    if self.scale < 0.5 or self.scale > 2 then
        self.scalar = self.scalar * -1
    end
end

function test:draw()
    -- draw spinning/scaling image
    graphics.set_draw_color(1, 1, 1, 1)  
    local static_pos = { x = (640 * 0.3) - ((self.w * 0.5) * self.scale), y = (360 * 0.5) - ((self.h * 0.5) * self.scale) }    
    graphics.drawx(self.image, static_pos.x, static_pos.y, 0, 0, self.w, self.h, self.scale, self.scale, self.rotation)

    -- draw red image
    graphics.set_draw_color(1, 0, 0, 1)    
    local r_draw_pos = { x = (640 * 0.7) - ((self.w * 0.5) * 0.5), y = (360 * 0.15) - ((self.h * 0.5) * 0.5) }
    graphics.drawx(self.image, r_draw_pos.x, r_draw_pos.y, 0, 0, self.w, self.h, 0.5, 0.5, 0)

    -- draw green image
    graphics.set_draw_color(0, 1, 0, 1)    
    local g_draw_pos = { x = (640 * 0.7) - ((self.w * 0.5) * 0.5), y = (360 * 0.35) - ((self.h * 0.5) * 0.5) }
    graphics.drawx(self.image, g_draw_pos.x, g_draw_pos.y, 0, 0, self.w, self.h, 0.5, 0.5, 0)

    -- draw blue image
    graphics.set_draw_color(0, 0, 1, 1)    
    local b_draw_pos = { x = (640 * 0.7) - ((self.w * 0.5) * 0.5), y = (360 * 0.55) - ((self.h * 0.5) * 0.5) }
    graphics.drawx(self.image, b_draw_pos.x, b_draw_pos.y, 0, 0, self.w, self.h, 0.5, 0.5, 0)

    -- draw alpha image
    graphics.set_draw_color(1, 1, 1, 0.3)    
    local a_draw_pos = { x = (640 * 0.7) - ((self.w * 0.5) * 0.5), y = (360 * 0.75) - ((self.h * 0.5) * 0.5) }
    graphics.drawx(self.image, a_draw_pos.x, a_draw_pos.y, 0, 0, self.w, self.h, 0.5, 0.5, 0)
end

mainloop(test)