local window = require("milk.window")
local controller = require("milk.controller")
local buttons = controller.buttons;
local axes = controller.axes;

local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()

    if controller.is_button_released(buttons.A) then
        controller.rumble(1.0, 100);
    end
    if controller.is_button_released(buttons.B) then
        controller.rumble(0.75, 600);
    end
    if controller.is_button_released(buttons.Y) then
        controller.rumble(0.5, 1100);
    end
    if controller.is_button_released(buttons.X) then
        controller.rumble(0.25, 1600);
    end
    if controller.is_button_released(buttons.PAD_UP) then
        controller.rumble(0.25);
    end
    if controller.is_button_released(buttons.PAD_DOWN) then
        controller.stop_rumble();
    end
    
end

return test