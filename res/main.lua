local window = require "milk.window"
local renderer = require "milk.renderer"
local keyboard = require "milk.keyboard"
local KEYS = keyboard.KEYS

local callbacks = {};

local speed = 2
local x = 10
local y = 10

function callbacks.tick()
	if keyboard.is_key_released(KEYS.F) then 
		window.set_fullscreen(not window.is_fullscreen()) 
	end

	if keyboard.is_key_down(KEYS.A) then 
		x = x - speed
	end

	if keyboard.is_key_down(KEYS.W) then 
		y = y - speed
	end

	if keyboard.is_key_down(KEYS.S) then 
		y = y + speed
	end
	if keyboard.is_key_down(KEYS.D) then 
		x = x + speed
	end

	renderer.draw_rect(x, y, 50, 50);
end

return callbacks