local window = require("milk.window")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local keyboard = require("milk.keyboard")
local Gui = require("res.gui")
local keys = keyboard.keys

-- buttons
local TOGGLE_FULLSCREEN = { x = 64, y = 0, w = 64, h = 64 }
local TOGGLE_MUSIC = { x = 320, y = 0, w = 64, h = 64 }

local state = {
    menu_sheet = graphics.new_texture("res/menusheet.png"),
    gui = Gui.new()
}

function state:enter(game)
end

function state:exit(game)
end

function state:tick(game)
    if keyboard.is_key_down(keys.ESCAPE) then
        game.pop_state()
    end
end

function state:draw(game)
    -- black overlay
    graphics.set_draw_color(0, 0, 0, 0.7)
    graphics.draw_filled_rect(0, 0, 640, 360)

    self.gui:start()
    if self.gui:button(1, 400, 120, self.menu_sheet, TOGGLE_FULLSCREEN) then
        local toggle = not window.is_fullscreen()
        window.set_fullscreen(toggle)
    end
    if self.gui:button(2, 400, 190, self.menu_sheet, TOGGLE_MUSIC) then
        audio.stop_music()
    end
    self.gui:stop()
end

return state