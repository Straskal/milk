local window = require("milk.window")
local graphics = require("milk.graphics")
local Gui = require("res.gui")
local settings_state = require("res.state_settings")

-- buttons
local RESUME = { x = 0, y = 0, w = 64, h = 64 }
local SETTINGS = { x = 256, y = 0, w = 64, h = 64 }
local QUIT = { x = 192, y = 0, w = 64, h = 64 }

local state = {
    menu_sheet = graphics.new_texture("res/menusheet.png"),
    gui = Gui.new()
}

function state:enter(game)
end

function state:exit(game)
end

function state:tick(game)
end

function state:draw(game)
    -- draw black  overlay
    graphics.set_draw_color(0, 0, 0, 0.7)
    graphics.draw_filled_rect(0, 0, 640, 360)
    
    self.gui:start()
    if self.gui:button(1, 300, 50, self.menu_sheet, RESUME) then
        game.pop_state()
    end
    if self.gui:button(2, 300, 120, self.menu_sheet, SETTINGS) then
        game.push_state(settings_state)
    end      
    if self.gui:button(3, 300, 190, self.menu_sheet, QUIT) then
        window.close()
    end
    self.gui:stop()
end

return state