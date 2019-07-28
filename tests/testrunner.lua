local milk = require("milk")
local window = require("milk.window")
local time = require("milk.time")
local graphics = require("milk.graphics")

--------------------------------------------
local tests = {
    WINDOW = "tests/windowtest.lua",
    GRAPHICS = "tests/graphicstest.lua",
    MOUSE = "tests/mousetest.lua",
    KEYBOARD = "tests/keyboardtest.lua",
    CONTROLLER = "tests/controllertest.lua",
    RUMBLE = "tests/rumbletest.lua",
    LISTENERS = "tests/listenertest.lua"
}
--------------------------------------------
--------------------------------------------
local function run(test)
    local start = test.start or function()
        end
    local tick = test.tick or function()
        end
    local draw = test.draw or function()
        end
    local stop = test.stop or function()
        end

    milk.init()

    -- start test
    start(test)

    window.show()

    local TARGET_FPS = 60
    local SECONDS_PER_TICK = 1 / TARGET_FPS
    local frame_start_time = 0
    local frame_time
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

local function run_test(name, file)
    print("=================================")
    print("RUNNING " .. name)
    print("=================================")
    run(dofile(file))
end
--------------------------------------------
--------------------------------------------
local test_to_run
repeat
    print("TEST RUNNER")
    print("=================================")
    io.write("Enter test name: ")
    io.flush()
    test_to_run = string.upper(io.read())
    if test_to_run == "RUN_ALL" then
        for test_name, test_file in pairs(tests) do
            run_test(test_name, test_file)
        end
    elseif tests[test_to_run] then
        run_test(test_to_run, tests[test_to_run])
    end
until test_to_run == "STOP"
