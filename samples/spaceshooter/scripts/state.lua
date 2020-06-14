local class = require "scripts.class"

-- Base class for all game states.
local GameState = class("GameState")

-- luacheck: push ignore self game
function GameState:enter(game)
end

function GameState:update(game)
end

function GameState:draw(game)
end

function GameState:exit(game)
end
-- luacheck: pop

return GameState