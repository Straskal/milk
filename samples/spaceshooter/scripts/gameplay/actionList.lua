local class = require "scripts.class"

local GameplayActionList = class("GameplayActionList")

function GameplayActionList:initialize(gameplayActions)
    self.actions = gameplayActions
    self.currentAction = 1
end

-- luacheck: push ignore self game gameplay
function GameplayActionList:update(game, gameplay)
    if (self.actions[self.currentAction]:update(game, gameplay)) then
        self.currentAction = self.currentAction + 1
    end
    return self.currentAction > #self.actions
end

function GameplayActionList:draw(game, gameplay)
    self.actions[self.currentAction]:draw(game, gameplay)
end
-- luacheck: pop

return GameplayActionList