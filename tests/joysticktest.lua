local window = require("milk.window")
local joystick = require("milk.joystick")

local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()
    -- down
    if joystick.get_x() == 1 then
        print("Joystick right")
    end
    if joystick.get_x() == -1 then
        print("Joystick left")
    end
    if joystick.get_y() == 1 then
        print("Joystick up")
    end
    if joystick.get_y() == -1 then
        print("Joystick down")
    end
end

return test