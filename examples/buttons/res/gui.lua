local mouse = require("milk.mouse")
local keyboard = require("milk.keyboard")
local graphics = require("milk.graphics")
local mouse_buttons = mouse.buttons

-- helpers
local is_mouse_hover = function(x, y, w, h)
    local mx, my = mouse.get_position()
    return mx > x and mx <= x + w and my > y and my <= y + h
end

-- state
local mousex = 0
local mousey = 0
local isdown = false
local hot_id = -1
local active_id = -1

-- styling
local hot_color = { 0.1, 1, 1, 1 }
local active_color = { 0.8, 1, 1, 1 }

local gui = {}

function gui.start()
    hot_id = 0
    isdown = mouse.is_button_down(mouse_buttons.LEFT)
end

function gui.stop()
    if not isdown then 
        active_id = 0
    elseif active_id == 0 then
        active_id = -1        
    end
end 

function gui.button(id, x, y, texture, src_rect)
    if is_mouse_hover(x, y, src_rect.w, src_rect.h) then
        hot_id = id
        if active_id == 0 and isdown then
            active_id = id
        end
    end
    if hot_id == id then
        if active_id == id then
            graphics.set_draw_color(table.unpack(active_color))
            graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
        else
            graphics.set_draw_color(table.unpack(hot_color))
            graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
        end
    else
        graphics.set_draw_color(1, 1, 1, 1)
        graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
    end
    -- if button is not down but control is hot and active then the user has clicked this button
    return not isdown and hot_id == id and active_id == id
end

return gui