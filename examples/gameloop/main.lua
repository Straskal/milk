local milk = require("milk")
local window = require("milk.window")
local time = require("milk.time")
local graphics = require("milk.graphics")
local game = require("res.game")

milk.init()

local start = game.start or function(game) end
local tick = game.tick or function(game) end
local draw = game.draw or function(game) end
local stop = game.stop or function(game) end

-- initialize
start(game)

window.show()

local TARGET_FPS = 60
local SECONDS_PER_TICK = 1 / TARGET_FPS
local frame_start_time = 0
local frame_time = 0
local accumulated_frame_time = 0

while not window.should_close() do
    local t = time.get_total()
    frame_time = t - frame_start_time
    frame_start_time = t
    accumulated_frame_time = accumulated_frame_time + frame_time

    while accumulated_frame_time >= SECONDS_PER_TICK do
        window.poll()

        -- game logic
        tick(game, SECONDS_PER_TICK)

        graphics.set_draw_color(0, 0, 0, 1)
        graphics.clear()
        graphics.set_draw_color(1, 1, 1, 1)
        
        -- draw logic
        draw(game, SECONDS_PER_TICK)
        
        graphics.present()
        accumulated_frame_time = accumulated_frame_time - SECONDS_PER_TICK
    end
end

-- shutdown
stop(game)

milk.quit()
