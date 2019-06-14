local video = require("milk.video")
local input = require("milk.input")

local keys = input.keyboard.keys

-- initialize game
video.window.set_title("Butt Dragons")
video.window.set_size(1280, 720)
video.renderer.set_resolution(640, 360)

local playertexture = video.texture.new("res/player.png")
local playerposition = { x = 0, y = 0 }
local sourcerectangle = { x = 0, y = 0, w = 64, h = 64 }
local lastanimtime = 0
local SECONDS_PER_ANIM_FRAME = 0.1

-- callback table
local callbacks = {};

-- game logic goes here
function callbacks.tick()
	if input.keyboard.was_key_pressed(keys.F)
		local toggle = not video.window.is_fullscreen()
		video.window.set_fullscreen(toggle)
	end

	local totaltime = os.clock()
	if totaltime - lastanimtime > SECONDS_PER_ANIM_FRAME then
		sourcerectangle.x = sourcerectangle.x + 64
		if (sourcerectangle.x >= 512) then
			sourcerectangle.x = 0
		end
		lastanimtime = totaltime
	end
end

-- render calls go here
function callbacks.render()
	video.renderer.drawex(playertexture, playerposition, sourcerectangle)
end

return callbacks