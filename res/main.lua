local milk = require "milk"

local maintable = {};

function maintable:tick()
	if milk.is_key_down("f") then milk.toggle_fullscreen() end
end

return maintable