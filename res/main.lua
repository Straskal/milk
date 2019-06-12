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
	position = { x = 0, y = 0 },
	speed = 2
}

-- callback table
local callbacks = {};

function callbacks.tick(dt)
	if keyboard.was_key_released(keys.F) then 
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle) 
	end

	if keyboard.was_key_released(keys.ESCAPE) then 
		window.close()
	end

	if keyboard.was_key_pressed(keys.L) then
		local w, h = window.get_size()
		print("window size:" .. " " .. w .. ", " .. h)
	end

	local position = player.position
	if keyboard.is_key_pressed(keys.W) then position.y = position.y - player.speed * dt end
	if keyboard.is_key_pressed(keys.A) then position.x = position.x - player.speed * dt end
	if keyboard.is_key_pressed(keys.S) then position.y = position.y + player.speed * dt end
	if keyboard.is_key_pressed(keys.D) then position.x = position.x + player.speed * dt end
end

function callbacks.render(dt)
	renderer.draw(player.texture, player.position, { x = 0, y = 0, w = 64, h = 64 })
end

return callbacks