local milk = require("scripts.milk")
local introState = require("scripts.states.intro.intro")

local game = {
    stateStack = {}
}

function game.init()
    game.pushState(introState)
end

function game.update()
    local len = #game.stateStack
    for i = len, 1, -1 do
        game.stateStack[i].update(game)
    end
end

function game.draw()
    milk.clrs(0x1a1a1a)

    local len = #game.stateStack
    for i = 1, len do
        game.stateStack[i].draw(game)
    end
end

function game.pushState(state)
    table.insert(game.stateStack, state)
    if state.enter then
        state.enter(game)
    end
end

function game.popState()
    local len = #game.stateStack
    if len > 0 then
        if game.stateStack[len].exit then
            game.stateStack[len].exit(game)
        end
        table.remove(game.stateStack)
    end
end

function game.clearState()
    game.stateStack = {}
end

return game
