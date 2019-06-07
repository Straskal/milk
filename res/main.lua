local window = require "milk.window"
local keyboard = require "milk.keyboard"
local KEYS = keyboard.KEYS

local maintable = {};

function maintable:tick()
	if keyboard.is_key_released(KEYS.F) then 
		window.set_fullscreen(not window.is_fullscreen()) 
	end
end

return maintable