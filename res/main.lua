local video = require "milk.video"
local input = require "milk.input"

-- submodules
local window = video.window
local renderer = video.renderer
local textures = video.textures
local keyboard = input.keyboard
local keys = keyboard.keys

-- initialize game
window.set_title("Butt Dragons")
window.set_size(1280, 720)
renderer.set_resolution(640, 360)

local player = {
	texture = textures.load("res/player.png"),
	speed = 2,
	x = 10,
	y = 10
}

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
		print("window size:" .. " " .. w .. ", " .. h)
	end

	if keyboard.is_key_pressed(keys.O) then
		local w, h = player.texture:get_size()
		print("texture size:" .. " " .. w .. ", " .. h)
	end

	if keyboard.is_key_down(keys.W) then player.y = player.y - player.speed end
	if keyboard.is_key_down(keys.A) then player.x = player.x - player.speed end
	if keyboard.is_key_down(keys.S) then player.y = player.y + player.speed end
	if keyboard.is_key_down(keys.D) then player.x = player.x + player.speed end
end

function callbacks.render()
	renderer.draw(player.texture, player.x, player.y);
end

return callbacks