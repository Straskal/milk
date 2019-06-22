local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local keys = keyboard.keys

local gui = require("res.gui")

-- initialize game
window.set_title("UI test")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local game = {};
game.music = audio.new_music("res/music.mp3")
game.sound = audio.new_sound("res/sound.wav")
game.button_sheet = graphics.new_texture("res/buttonsheet.png")

local buttonpadding = 10
local buttonsw = (64 * 3) + (buttonpadding * 2)
local buttonsposx = (640 / 2) - (buttonsw / 2)
local buttonsposy = (360 / 2) - (64 / 2)

function game.tick()
	if keyboard.is_key_released(keys.ESCAPE) then 
		window.close() 
	end
	if keyboard.is_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end
end

function game.draw()
	gui.start()
	if gui.button(1, buttonsposx, buttonsposy, game.button_sheet, { x = 0, y = 0, w = 64, h = 64 }) then
		audio.play_sound(game.sound)
	end
	if gui.button(2, buttonsposx + 64 + buttonpadding, buttonsposy, game.button_sheet, { x = 64, y = 0, w = 64, h = 64 }) then
		audio.loop_music(game.music, 1)
	end
	if gui.button(3, buttonsposx + 128 + buttonpadding * 2, buttonsposy, game.button_sheet, { x = 128, y = 0, w = 64, h = 64 }) then
		audio.stop_music()
	end
	gui.stop()
end

return game