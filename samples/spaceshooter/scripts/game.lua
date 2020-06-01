local milk = require("scripts.milk")
local introState = require("scripts.states.introstate")

local game = {
    stateStack = {}
}

function game.init()
    game.pushState(introState)
end

function game.update()
    local len = #game.stateStack
    for i = len, 1, -1 do
        game.stateStack[i].update()
    end
end

function game.draw()
    milk.clrs(0x1a1a1a)

    local len = #game.stateStack
    for i = 1, len do
        game.stateStack[i].draw()
    end
end

function game.pushState(state)
    table.insert(game.stateStack, state)
end

function game.popState()
    if #game.stateStack > 0 then
        table.remove(game.stateStack)
    end
end

function game.clearState()
    game.stateStack = {}
end

return game
