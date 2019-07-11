local window = require("milk.window")
local mouse = require("milk.mouse")
local keyboard = require("milk.keyboard")
local buttons = mouse.buttons
local keys = keyboard.keys

--[[
    the draw test proves the following:
    - that the keyboard works lol
--]]
local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()
    -- down
    if keyboard.is_key_down(keys.LSHIFT) then
        if mouse.is_button_down(buttons.LEFT) then
            print("LEFT button down")
        end
        if mouse.is_button_down(buttons.MIDDLE) then
            print("MIDDLE button down")
        end
        if mouse.is_button_down(buttons.RIGHT) then
            print("RIGHT button down")
        end
    end

    -- press
    if mouse.is_button_pressed(buttons.LEFT) then
        print("LEFT button pressed")
        local x, y = mouse.get_position()
        print("X: " .. x .. " Y: " .. y)
    end
    if mouse.is_button_pressed(buttons.MIDDLE) then
        print("MIDDLE button pressed")
    end
    if mouse.is_button_pressed(buttons.RIGHT) then
        print("RIGHT button pressed")
    end

    -- release
    if mouse.is_button_released(buttons.LEFT) then
        print("LEFT button released")
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