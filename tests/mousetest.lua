local window = require("milk.window")
local mouse = require("milk.mouse")
local buttons = mouse.buttons

--[[
    the draw test proves the following:
    - that the keyboard works lol
--]]
local test = {}

function test:start()
    window.set_size(1280, 720)
end

function test:tick()
    if mouse.is_button_released(buttons.LEFT) then
        print("LEFT button released")
        local x, y = mouse.get_position()
        print("X: " .. x .. " Y: " .. y)
    end
    if mouse.is_button_released(buttons.MIDDLE) then
        print("MIDDLE button released")
    end
    if mouse.is_button_released(buttons.RIGHT) then
        print("RIGHT button released")
    end
    local scroll = mouse.get_scroll()
    if scroll ~= 0 then
        print("Scroll: " .. scroll)
    end
end

return test