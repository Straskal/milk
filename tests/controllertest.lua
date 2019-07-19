local window = require("milk.window")
local controller = require("milk.controller")
local buttons = controller.buttons;
local axes = controller.axes;

local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()
    -- left joystick
    if controller.get_axis(axes.LEFT_X) > 0.2 or controller.get_axis(axes.LEFT_X) < -0.2 then
        print(controller.get_axis(axes.LEFT_X))
    end
    if controller.get_axis(axes.LEFT_Y) > 0.2 or controller.get_axis(axes.LEFT_Y) < -0.2 then
        print(controller.get_axis(axes.LEFT_Y))
    end

    -- right joystick
    if controller.get_axis(axes.RIGHT_X) > 0.2 or controller.get_axis(axes.RIGHT_X) < -0.2 then
        print(controller.get_axis(axes.RIGHT_X))
    end
    if controller.get_axis(axes.RIGHT_Y) > 0.2 or controller.get_axis(axes.RIGHT_Y) < -0.2 then
        print(controller.get_axis(axes.RIGHT_Y))
    end

    -- triggers
    if controller.get_axis(axes.LEFT_TRIGGER) > 0.2 then
        print(controller.get_axis(axes.LEFT_TRIGGER))
    end
    if controller.get_axis(axes.RIGHT_TRIGGER) > 0.2 then
        print(controller.get_axis(axes.RIGHT_TRIGGER))
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
    if controller.is_button_released(buttons.BACK) then
        print("BACK")
    end
    if controller.is_button_released(buttons.GUIDE) then
        print("GUIDE")
    end
    if controller.is_button_released(buttons.START) then
        print("START")
    end
    if controller.is_button_released(buttons.LEFT_STICK) then
        print("LEFT_STICK")
    end
    if controller.is_button_released(buttons.RIGHT_STICK) then
        print("RIGHT_STICK")
    end
    if controller.is_button_released(buttons.SHOULDER_LEFT) then
        print("SHOULDER_LEFT")
    end
    if controller.is_button_released(buttons.SHOULDER_RIGHT) then
        print("SHOULDER_RIGHT")
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
end

return test