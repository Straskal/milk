local mouse = require("milk.mouse")
local graphics = require("milk.graphics")
local mouse_buttons = mouse.buttons

local layers = 0

local style = {
    button = {
        default_color = { 1, 1, 1, 1 },
        hot_color = { 0.1, 1, 1, 1 },
        active_color = { 0.8, 1, 1, 1 },
        disabled_color = { 0.3, 0.3, 0.3, 1 }
    }
}

-- helpers
local is_mouse_hover = function(mx, my, x, y, w, h)
    return mx > x and mx <= x + w and my > y and my <= y + h
end

local push_id = function()
    layers = layers + 1
    return layers
end

local pop_id = function()
    layers = layers - 1
end

local enabled = function(layer_id)
    return layer_id == layers
end

local new_layer = function()
    local layer_id = push_id()
    local mousex = 0
    local mousey = 0
    local is_down = false
    local hot_id = -1
    local active_id = -1

    local layer = {}
    
    function layer:begin_frame()
        hot_id = 0
        mousex, mousey = mouse.get_position()
        is_down = mouse.is_button_down(mouse_buttons.LEFT)
    end
    
    function layer:end_frame()
        if not is_down then 
            active_id = 0
        elseif active_id == 0 then
            active_id = -1        
        end
    end

    function layer:remove()
        layers = layers - 1
    end
    
    function layer:button(id, x, y, texture, src_rect)
        if not enabled(layer_id) then
            graphics.set_draw_color(table.unpack(style.button.disabled_color))
            graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
            return false
        end
        if is_mouse_hover(mousex, mousey, x, y, src_rect.w, src_rect.h) then
            hot_id = id
            if active_id == 0 and is_down then
                active_id = id
            end
        end
        if hot_id == id then
            if active_id == id then
                graphics.set_draw_color(table.unpack(style.button.active_color))
            else
                graphics.set_draw_color(table.unpack(style.button.hot_color))
            end
        else
            graphics.set_draw_color(table.unpack(style.button.default_color))
        end
        graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
        -- if button is not down but control is hot and active then the user has clicked this button
        return not is_down and hot_id == id and active_id == id
    end
    
    return layer
end

return {
    new_layer = new_layer,
    style = style
}