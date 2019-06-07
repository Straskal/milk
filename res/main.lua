local milk = require "milk"
local window = require "milk.window"

local maintable = {};

function maintable:tick()
	if milk.is_key_down("f") then 
		window.set_fullscreen(not window.is_fullscreen()) 
	end
end

return maintable