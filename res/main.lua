local video = require "milk.video"
local input = require "milk.input"

-- submodules
local window = video.window
local renderer = video.renderer
local keyboard = input.keyboard
local keys = keyboard.keys

-- initialize game
window.set_title("Butt Dragons")
window.set_size(1280, 720)
renderer.set_resolution(640, 360)

local speed = 2
local x = 10
local y = 10

-- callback table
local callbacks = {};

function callbacks.tick()
	if keyboard.is_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end

	if keyboard.is_key_released(keys.ESC) then 
		window.close()
	end

	if keyboard.is_key_pressed(keys.L) then
		local w, h = window.get_size()
		print("size:" .. " " .. w .. ", " .. h)
	end

	if keyboard.is_key_down(keys.W) then y = y - speed end
	if keyboard.is_key_down(keys.A) then x = x - speed end
	if keyboard.is_key_down(keys.S) then y = y + speed end
	if keyboard.is_key_down(keys.D) then x = x + speed end
end

function callbacks.render()
	renderer.draw_rect(x, y, 50, 50);
end

return callbacks