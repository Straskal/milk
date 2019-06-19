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
local sound = audio.new_sound("res/sound.wav");
local music = audio.new_music("res/music.mp3")
local player_pos = { x = 0, y = 0 }
local source_rect = { x = 0, y = 0, w = 64, h = 64 }
local last_anim_time = 0
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
	if keyboard.was_key_pressed(keys.SPACE) then audio.play_sound(sound) end
	if keyboard.was_key_pressed(keys.NUM1) then audio.play_music(music) end
	if keyboard.was_key_pressed(keys.NUM2) then audio.stop_music() end
	if keyboard.is_key_pressed(keys.W) then player_pos.y = player_pos.y - 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.A) then player_pos.x = player_pos.x - 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.S) then player_pos.y = player_pos.y + 1 * PLAYER_SPEED end
	if keyboard.is_key_pressed(keys.D) then player_pos.x = player_pos.x + 1 * PLAYER_SPEED end

	local totaltime = os.clock()
	if totaltime - last_anim_time > SECONDS_PER_ANIM_FRAME then
		source_rect.x = source_rect.x + 64
		if (source_rect.x >= 512) then
			source_rect.x = 0
		end
		last_anim_time = totaltime
	end
end

-- draw calls go here
function callbacks.draw()
	graphics.drawex(player_texture, player_pos, source_rect)
end

return callbacks