local starField = require("starfield")

function _init()
	starField.init()
end

function _update()
	starField.update()
end

function _draw()	
	starField.draw()
end

function _cheat(cmd, args) 
	if cmd == "health" then
		-- giveHealth(args[0])
	end
end
