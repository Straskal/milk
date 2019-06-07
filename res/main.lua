local window = require "milk.window"
local keyboard = require "milk.keyboard"
local keys = keyboard.keys

local maintable = {};

function maintable:tick()
	if keyboard.is_key_released(keys.F) then 
		window.set_fullscreen(not window.is_fullscreen()) 
	end
end

return maintable