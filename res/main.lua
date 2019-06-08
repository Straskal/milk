local window = require "milk.window"
local renderer = require "milk.renderer"
local keyboard = require "milk.keyboard"
local KEYS = keyboard.KEYS

window.set_title("Butt Dragons")
window.set_size(1280, 720)
renderer.set_resolution(640, 360)

local speed = 2
local x = 10
local y = 10

local callbacks = {};

function callbacks.tick()
	if keyboard.is_key_released(KEYS.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end

	if keyboard.is_key_pressed(KEYS.L) then
		local w, h = milkwindow.get_size()
		print("size:" .. " " .. w .. ", " .. h)
	end

	if keyboard.is_key_down(KEYS.W) then y = y - speed end
	if keyboard.is_key_down(KEYS.A) then x = x - speed end
	if keyboard.is_key_down(KEYS.S) then y = y + speed end
	if keyboard.is_key_down(KEYS.D) then x = x + speed end

	renderer.draw_rect(x, y, 50, 50);
end

return callbacks