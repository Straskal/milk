local window = require("milk.window")
local time = require("milk.time")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local player = require("res.player")
local keys = keyboard.keys

local game = {};

function game:start()
	window.set_title("Animation")
	window.set_size(1280, 720)
	graphics.set_virtual_resolution(640, 360)

	self.player = player.new()
end

function game:tick()
	if keyboard.is_key_released(keys.ESCAPE) then 
		window.close() 
	end
	if keyboard.is_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end

	self.player:tick()
end

function game:draw()
	self.player:draw()
end

return game