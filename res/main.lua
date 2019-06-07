local window = require "milk.window"
local keyboard = require "milk.keyboard"

local maintable = {};

function maintable:tick()
	if keyboard.is_key_down("f") then 
		window.set_fullscreen(not window.is_fullscreen()) 
	end
end

return maintable