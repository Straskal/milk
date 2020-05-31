local milk = require("milk")
local starField = require("starfield")
local player = require("playerShip").new()

function milk.init()
	starField.init()
end

function milk.update()
	starField.update()
	player:update()
end

function milk.draw()
	milk.clrs(0x1a1a1a)
	starField.draw()
	player:draw()
end
