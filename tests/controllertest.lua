local window = require("milk.window")
local controller = require("milk.controller")
local buttons = controller.buttons;

local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()
    -- left joystick
    if controller.get_leftx() == 1 then
        print("Left stick - right")
    end
    if controller.get_leftx() == -1 then
        print("Left stick - left")
    end
    if controller.get_lefty() == 1 then
        print("Left stick - down")
    end
    if controller.get_lefty() == -1 then
        print("left stick - up")
    end

    -- right joystick
    if controller.get_rightx() == 1 then
        print("Right stick - right")
    end
    if controller.get_rightx() == -1 then
        print("Right stick - left")
    end
    if controller.get_righty() == 1 then
        print("Right stick - down")
    end
    if controller.get_righty() == -1 then
        print("Right stick - up")
    end

    --buttons
    if controller.is_button_released(buttons.A) then
        print("A")
    end
    if controller.is_button_released(buttons.B) then
        print("B")
    end
    if controller.is_button_released(buttons.X) then
        print("X")
    end
    if controller.is_button_released(buttons.Y) then
        print("Y")
    end
    if controller.is_button_released(buttons.PAD_UP) then
        print("PAD_UP")
    end
    if controller.is_button_released(buttons.PAD_DOWN) then
        print("PAD_DOWN")
    end
    if controller.is_button_released(buttons.PAD_LEFT) then
        print("PAD_LEFT")
    end
    if controller.is_button_released(buttons.PAD_RIGHT) then
        print("PAD_RIGHT")
    end
    if controller.is_button_released(buttons.BACK) then
        print("BACK")
    end
    if controller.is_button_released(buttons.START) then
        print("START")
    end
    if controller.is_button_released(buttons.SHOULDER_LEFT) then
        print("SHOULDER_LEFT")
    end
    if controller.is_button_released(buttons.SHOULDER_RIGHT) then
        print("SHOULDER_RIGHT")
    end
end

return test