local GameplayAction = require "scripts.gameplay.action"
local DialogueBox = require "scripts.common.dialogue"

local Introduction = GameplayAction:subclass("Introduction")

function Introduction:initialize()
    self.dialogueBox = DialogueBox(48, "FUCK YOU!")
end

function Introduction:update()
    self.dialogueBox:update()
    return DialogueBox.isFinished
end

function Introduction:draw()
    self.dialogueBox:draw()
end

return Introduction