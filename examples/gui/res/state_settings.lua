local window = require("milk.window")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local keyboard = require("milk.keyboard")
local gui = require("res.gui")
local keys = keyboard.keys

-- buttons
local TOGGLE_FULLSCREEN = { x = 128, y = 0, w = 64, h = 32 }
local PLAY_MUSIC = { x = 192, y = 0, w = 64, h = 32 }
local STOP_MUSIC = { x = 256, y = 0, w = 64, h = 32 }
local PLAY_SOUND = { x = 320, y = 0, w = 64, h = 32 }

local state = {
    menu_sheet = graphics.new_image("res/menusheet.png"),
    sound = audio.new_sound("res/sound.wav"),
    music = audio.new_music("res/music.mp3")
}

function state:enter(game)
    self.gui_layer = gui.new_layer()
end

function state:exit(game)
    self.gui_layer:remove()
end

function state:tick(game)
    if keyboard.is_key_down(keys.ESCAPE) then
        game.pop_state()
    end
end

function state:draw(game)
    self.gui_layer:begin_frame()
    if self.gui_layer:button(1, 400, 120, self.menu_sheet, TOGGLE_FULLSCREEN) then
        local toggle = not window.is_fullscreen()
        window.set_fullscreen(toggle)
    end
    if self.gui_layer:button(2, 400, 190, self.menu_sheet, PLAY_SOUND) then
		self.sound:play()
    end
    if self.gui_layer:button(3, 500, 120, self.menu_sheet, PLAY_MUSIC) then
		self.music:loop(1)
    end
    if self.gui_layer:button(4, 500, 190, self.menu_sheet, STOP_MUSIC) then
        self.music:stop(1)
    end
    self.gui_layer:end_frame()
end

return state