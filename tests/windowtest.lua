local window = require("milk.window")
local time = require("milk.time")

local stages = {
    {
        start = function()
            print("setting size to 1280x720")
            window.set_size(1280, 720)
        end
    },
    {
        start = function()
            print("setting title to 'new title!'")
            window.set_title("new title!")
        end
    },
    {
        start = function()
            print("setting icon")
            window.set_icon("tests/assets/milkicon.png")
        end
    },
    {
        start = function()
            print("setting fullscreen to true")
            window.set_fullscreen(true)
        end
    },
    {
        start = function()
            print("setting fullscreen to false")
            window.set_fullscreen(false)
        end
    }
}

--[[
    the draw test proves the following:
    - the window's size settings work
    - the window can be minimized
    - the window can be restored
    - the window's fullscreen setting can be toggled
--]]
local test = {}

function test:start()
    self.stage_num = 1
    self.stage = stages[self.stage_num]
    self.stage.start()
    self.stage.start_time = time.get_total() + 3
end

function test:tick()
    local t = time.get_total()
    if t > self.stage.start_time then
        self.stage_num = self.stage_num + 1
        if self.stage_num > #stages then
            window.close()
        else
            self.stage = stages[self.stage_num]
            self.stage.start()
            self.stage.start_time = time.get_total() + 3
        end
    end
end

function test:draw()
end

function test:stop()
end

return test