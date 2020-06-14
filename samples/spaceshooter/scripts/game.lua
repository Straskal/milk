local milk = require("scripts.milk")
local IntroState = require("scripts.intro.state")

local game = {
    stateStack = {}
}

function game.init()
    game.pushState(IntroState())
end

function game.update()
    local len = #game.stateStack
    for i = len, 1, -1 do
        game.stateStack[i]:update(game)
    end
end

function game.draw()
    milk.clrs(0x1a1a1a)

    local len = #game.stateStack
    for i = 1, len do
        game.stateStack[i]:draw(game)
    end
end

function game.pushState(state)
    table.insert(game.stateStack, state)
    state:enter(game)
end

function game.popState()
    local len = #game.stateStack
    if len > 0 then
        game.stateStack[len]:exit(game)
        table.remove(game.stateStack)
    end
end

return game
