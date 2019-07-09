local milk = require("milk")
local window = require("milk.window")
local time = require("milk.time")
local graphics = require("milk.graphics")

local function run(test)
    local start = test.start or function() end
    local tick = test.tick or function() end
    local draw = test.draw or function() end
    local stop = test.stop or function() end

    milk.init()

    -- start test
    start(test)

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
            tick(test, SECONDS_PER_TICK)
    
            graphics.set_draw_color(0, 0, 0, 1)
            graphics.clear()
            graphics.set_draw_color(1, 1, 1, 1)
            
            draw(test, SECONDS_PER_TICK)
            
            graphics.present()
            accumulated_frame_time = accumulated_frame_time - SECONDS_PER_TICK
        end
    end

    stop(test)
    
    milk.quit()
end

return run