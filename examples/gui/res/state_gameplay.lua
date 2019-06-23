local keyboard = require("milk.keyboard")
local audio = require("milk.audio")
local Player = require("res.player")
local paused_state = require("res.state_menu")
local keys = keyboard.keys

local state = {
    player = Player.new(),
    music = audio.new_music("res/music.mp3")
}

state.player:init()

function state:enter(game)
    audio.loop_music(self.music, 1)
end

function state:exit(game)
    audio.stop_music()
end

function state:tick(game)
    if keyboard.is_key_pressed(keys.ESCAPE) then
        game.push_state(paused_state)
    end
    self.player:tick()
end

function state:draw(game)
    self.player:draw()
end

return state