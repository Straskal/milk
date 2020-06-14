local class = require "scripts.class"

local GameplayAction = class("GameplayAction")

-- luacheck: push ignore self game gameplay
function GameplayAction:enter(game, gameplay)
end

function GameplayAction:update(game, gameplay)
    return true
end

function GameplayAction:draw(game, gameplay)
end

function GameplayAction:exit(game, gameplay)
end
-- luacheck: pop

return GameplayAction