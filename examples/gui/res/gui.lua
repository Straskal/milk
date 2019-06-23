local mouse = require("milk.mouse")
local graphics = require("milk.graphics")
local mouse_buttons = mouse.buttons

-- helpers
local is_mouse_hover = function(mx, my, x, y, w, h)
    return mx > x and mx <= x + w and my > y and my <= y + h
end

local new = function()
    local gui = {
        -- state
        mousex = 0,
        mousey = 0,
        is_down = false,
        hot_id = -1,
        active_id = -1,
        -- styling
        hot_color = { 0.1, 1, 1, 1 },
        active_color = { 0.8, 1, 1, 1 }
    }
    
    function gui.start(self)
        self.hot_id = 0
        self.mousex, self.mousey = mouse.get_position()
        self.is_down = mouse.is_button_down(mouse_buttons.LEFT)
    end
    
    function gui.stop(self)
        if not self.is_down then 
            self.active_id = 0
        elseif self.active_id == 0 then
            self.active_id = -1        
        end
    end 
    
    function gui.button(self, id, x, y, texture, src_rect)
        if is_mouse_hover(self.mousex, self.mousey, x, y, src_rect.w, src_rect.h) then
            self.hot_id = id
            if self.active_id == 0 and self.is_down then
                self.active_id = id
            end
        end
        if self.hot_id == id then
            if self.active_id == id then
                graphics.set_draw_color(table.unpack(self.active_color))
            else
                graphics.set_draw_color(table.unpack(self.hot_color))
            end
        else
            graphics.set_draw_color(1, 1, 1, 1)
        end
        graphics.drawx(texture, x, y, src_rect.x, src_rect.y, src_rect.w, src_rect.h, 1, 0)
        -- if button is not down but control is hot and active then the user has clicked this button
        return not self.is_down and self.hot_id == id and self.active_id == id
    end
    
    return gui
end

return {
    new = new
}