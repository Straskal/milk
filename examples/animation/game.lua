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

function game:tick(dt)
	if keyboard.is_key_released(keys.ESCAPE) then 
		window.close() 
	end
	if keyboard.is_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end
  if keyboard.is_key_released(keys.Z) then 
		time.delay(5)
	end

	self.player:tick(dt)
end

function game:draw(dt)
	self.player:draw(dt)
end

function game:stop()
  print("milk ith thopping!!!")
end

return game