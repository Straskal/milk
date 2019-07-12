local milk = require("milk")
local window = require("milk.window")
local time = require("milk.time")
local graphics = require("milk.graphics")

-- initialize milk and it's libraries
milk.init()

-- initialize

window.show()

local TARGET_FPS = 60
local SECONDS_PER_TICK = 1 / TARGET_FPS
local frame_start_time = 0
local frame_time = 0
local accumulated_frame_time = 0

-- run at fixed time step of SECONDS_PER_TICK
while not window.should_close() do
    local t = time.get_total()
    frame_time = t - frame_start_time
    frame_start_time = t
    accumulated_frame_time = accumulated_frame_time + frame_time
    
    -- we most likely hit a breakpoint if a complete frame takes a whole second.
    if accumulated_frame_time > 1 then
        accumulated_frame_time = 1
    end

    while accumulated_frame_time >= SECONDS_PER_TICK do
        window.poll()

        -- game logic

        graphics.set_draw_color(0, 0, 0, 1)
        graphics.clear()
        graphics.set_draw_color(1, 1, 1, 1)
        
        -- draw logic
        
        graphics.present()
        accumulated_frame_time = accumulated_frame_time - SECONDS_PER_TICK
    end
end

-- shutdown

-- this should always be the last line of code executed.
milk.quit()