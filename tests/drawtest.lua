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
    - images are properly rotated
--]]
local test = {}

function test:start()
    window.set_size(1280, 720)
    window.set_icon("tests/assets/milkicon.png")
    graphics.set_resolution(640, 360)

    self.image = graphics.new_image("tests/assets/milk.png")
    self.w, self.h = self.image:get_size()
    self.position = { x = (640 * 0.5) - (self.w * 0.5), y = (360 * 0.5) - (self.h * 0.5) }
    self.rotation = 0
    self.scale = 1
end

function test:tick(dt)
    if keyboard.is_key_released(keys.ESCAPE) then
        window.close()
    end
    self.rotation = self.rotation + (50 * dt)
end

function test:draw()
    graphics.drawx(self.image, self.position.x, self.position.y, 0, 0, self.w, self.h, 1, 1, self.rotation)
end

mainloop(test)