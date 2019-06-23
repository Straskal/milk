local window = require("milk.window")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local audio = require("milk.audio")
local keys = keyboard.keys

local gameplay_state = require("res.state_gameplay")

-- initialize game
window.set_title("Game state test")
window.set_size(1280, 720)
graphics.set_virtual_resolution(640, 360)

local game = {
	states = {}
}

function game.push_state(state)
	table.insert(game.states, state)
	state:enter()
end

function game.pop_state()
	local top = game.states[#game.states]
	top:exit()
	table.remove(game.states)
end

game.push_state(gameplay_state)

function game.tick()
	local top = game.states[#game.states]
	top:tick(game)
end

function game.draw()
	local states = game.states
	for i = 1, #states do
		states[i]:draw(game)
	end
end

return game