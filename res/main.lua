local video = require "milk.video"
local input = require "milk.input"

-- submodules
local window = video.window
local renderer = video.renderer
local texture = video.texture
local keyboard = input.keyboard
local keys = keyboard.keys

-- initialize game
window.set_title("Butt Dragons")
window.set_size(1280, 720)
renderer.set_resolution(640, 360)

local tex = texture.new("res/player.png")
local positions = {}
local speed = 2
for i = 1, 100 do
	positions[i] = { x = i * 20, y = i * 20 }
end

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

	if keyboard.is_key_pressed(keys.W) then 
		for i = 1, #positions do
			positions[i].y = positions[i].y - speed
		end
	end
	if keyboard.is_key_pressed(keys.A) then 
		for i = 1, #positions do
			positions[i].x = positions[i].x - speed
		end
	end

	if keyboard.is_key_pressed(keys.S) then 
		for i = 1, #positions do
			positions[i].y = positions[i].y + speed
		end
	end

	if keyboard.is_key_pressed(keys.D) then 
		for i = 1, #positions do
			positions[i].x = positions[i].x + speed
		end
	end
end

function callbacks.render()
	for i = 1, #positions do		
		renderer.draw(tex, positions[i], { x = 0, y = 0, w = 64, h = 64 })
	end
end

return callbacks