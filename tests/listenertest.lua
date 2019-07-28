local window = require("milk.window")
local controller = require("milk.controller")
local messenger = require("milk.messenger")
local keyboard = require("milk.keyboard")
local keys = keyboard.keys

local test = {}

function test.start()
    window.set_size(1280, 720)
end

function test.tick()
    if keyboard.is_key_released(keys.UP) then
        print("Changing to tests/listeners")
        messenger.set_filepath("./tests/listeners.lua")
    end
    if keyboard.is_key_released(keys.DOWN) then
        print("Changing to scripts/listeners")
        messenger.set_filepath("./scripts/listeners.lua")
    end
end

return test