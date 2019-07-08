local window = require("milk.window")
local graphics = require("milk.graphics")
local state_menu = require("res.state_menu")

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

function game:start()
	window.set_title("GUI")
	window.set_size(1280, 720)
	graphics.set_resolution(640, 360)

	-- initial state
	game.push_state(state_menu)
end

function game.tick()
	-- only update the top state
	local top = game.states[#game.states]
	top:tick(game)
end

function game.draw()
	-- but draw all states
	for i = 1, #game.states do
		game.states[i]:draw(game)
	end
end

return game