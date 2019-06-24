local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local player = require("res.player")
local keys = keyboard.keys

-- initialization
window.set_title("Animation")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local game = {};

game.player = player.new()
game.player:init()

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