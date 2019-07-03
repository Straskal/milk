local window = require("milk.window")
local graphics = require("milk.graphics")
local gui = require("res.gui")
local settings_state = require("res.state_settings")

-- buttons
local SETTINGS = { x = 0, y = 0, w = 64, h = 32 }
local QUIT = { x = 64, y = 0, w = 64, h = 32 }

local state = {
    menu_sheet = graphics.new_image("res/menusheet.png")
}

function state:enter(game)
    self.gui_layer = gui.new_layer()
end

function state:exit(game)
end

function state:tick(game)
end

function state:draw(game)
    -- draw black overlay
    graphics.set_draw_color(0, 0, 0, 0.7)
    graphics.draw_filled_rect(0, 0, 640, 360)

    self.gui_layer:begin_frame()
    if self.gui_layer:button(1, 300, 120, self.menu_sheet, SETTINGS) then
        game.push_state(settings_state)
    end      
    if self.gui_layer:button(2, 300, 190, self.menu_sheet, QUIT) then
        window.close()
    end
    self.gui_layer:end_frame()
end

return state