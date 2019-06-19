local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local audio = require("milk.audio")

local keys = keyboard.keys

-- initialize game
window.set_title("Butt Dragons")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local player_texture = graphics.new_texture("res/player.png")
local player_sound = audio.new_sound("res/sound.wav");
local playerposition = { x = 0, y = 0 }
local sourcerectangle = { x = 0, y = 0, w = 64, h = 64 }
local lastanimtime = 0
local SECONDS_PER_ANIM_FRAME = 0.1
local PLAYER_SPEED = 2

-- callback table
local callbacks = {};

-- game logic goes here
function callbacks.tick()
	if keyboard.was_key_pressed(keys.F) then
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle)
	end

	if keyboard.was_key_released(keys.ESCAPE) then window.close() end
	if keyboard.was_key_pressed(keys.SPACE) then audio.play_sound(player_sound) end

	if keyboard.is_key_pressed(keys.W) then playerposition.y = playerposition.y - 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.A) then playerposition.x = playerposition.x - 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.S) then playerposition.y = playerposition.y + 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.D) then playerposition.x = playerposition.x + 1 * PLAYER_SPEED end

	local totaltime = os.clock()
	if totaltime - lastanimtime > SECONDS_PER_ANIM_FRAME then
		sourcerectangle.x = sourcerectangle.x + 64
		if (sourcerectangle.x >= 512) then
			sourcerectangle.x = 0
		end
		lastanimtime = totaltime
	end
end

-- draw calls go here
function callbacks.draw()
	graphics.drawex(player_texture, playerposition, sourcerectangle)
end

return callbacks