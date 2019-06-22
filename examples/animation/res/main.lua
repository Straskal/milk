local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local keys = keyboard.keys

local Player = require("res.player")

-- initialize game
window.set_title("test game")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local game = {};
game.player = Player.new()
game.music = audio.new_music("res/08 Ascending.mp3") -- by Eric Skiff

game.player:init()

local FADE_IN_SECONDS = 1
audio.loop_music(game.music, FADE_IN_SECONDS)

function game.tick()
	if keyboard.is_key_released(keys.ESCAPE) then 
		window.close() 
	end
	if keyboard.is_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end
	game.player:tick()
end

function game.draw()
	game.player:draw()
end

return game