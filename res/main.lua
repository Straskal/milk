local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local mouse = require("milk.mouse")

local keys = keyboard.keys
local mouse_buttons = mouse.buttons
local flip_flags = graphics.flip_flags

-- initialize game
window.set_title("Butt Dragons")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local player_texture = graphics.new_texture("res/player.png")
local sound = audio.new_sound("res/sound.wav");
local music = audio.new_music("res/music.mp3")
local player_flip = flip_flags.NONE

local player_pos = { x = 0, y = 0 }
local source_rect = { x = 0, y = 0, w = 64, h = 64 }

local last_anim_time = 0

local rect = { x = 10, y = 10, w = 300, h = 300 }
local rect_color = { r = 1, b = 0, g = 0, a = 1 }

local hover_over_player = false

local SECONDS_PER_ANIM_FRAME = 0.1
local PLAYER_SPEED = 2
local MUSIC_FADE_SECONDS = 1

-- callback table
local callbacks = {};

-- game logic goes here
function callbacks.tick()
	if keyboard.is_key_pressed(keys.F) then
		local toggle = not window.is_fullscreen()
		window.set_fullscreen(toggle)
	end

	print(mouse.get_scroll())

	if mouse.get_scroll() > 0 and rect_color.r < 1 then
		rect_color.r = rect_color.r + 0.1
	elseif mouse.get_scroll() < 0 and rect_color.r > 0.1 then
		rect_color.r = rect_color.r - 0.1
	end
	
	local mx, my = mouse.get_position()
	if mx <= player_pos.x + 64 and mx >= player_pos.x and my <= player_pos.y + 64 and my >= player_pos.y then
		if mouse.is_button_pressed(mouse_buttons.LEFT) then 
			audio.play_sound(sound)
		end
		hover_over_player = true
	else
		hover_over_player = false
	end

	if keyboard.is_key_released(keys.ESCAPE) then 
		window.close() 
	end

	if keyboard.is_key_pressed(keys.NUM1) then
		audio.loop_music(music, MUSIC_FADE_SECONDS) 
	end

	if keyboard.is_key_pressed(keys.NUM2) then 
		audio.stop_music(MUSIC_FADE_SECONDS) 
	end

	if keyboard.is_key_down(keys.W) then 
		player_pos.y = player_pos.y - 1 * PLAYER_SPEED
	end
	if keyboard.is_key_down(keys.S) then 
		player_pos.y = player_pos.y + 1 * PLAYER_SPEED 
	end
	if keyboard.is_key_down(keys.A) then 
		player_pos.x = player_pos.x - 1 * PLAYER_SPEED 
		player_flip = player_flip | flip_flags.X;
	end
	if keyboard.is_key_down(keys.D) then 
		player_pos.x = player_pos.x + 1 * PLAYER_SPEED
		player_flip = player_flip & ~flip_flags.X;
	end

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
	graphics.set_draw_color(rect_color.r, rect_color.b, rect_color.g, rect_color.a)
	graphics.draw_rect(rect.x, rect.y, rect.w, rect.h)
	if hover_over_player then
		graphics.draw_rect(player_pos.x, player_pos.y, source_rect.w, source_rect.h)
	end
	graphics.set_draw_color(1, 1, 1, 1)
	graphics.drawex(player_texture, player_pos.x, player_pos.y, source_rect.x, source_rect.y, source_rect.w, source_rect.h, player_flip)
end

return callbacks