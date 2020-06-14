local milk = require "scripts.milk"
local class = require "scripts.class"

local TIMER_TICKS = 5
local BOX_POSITION_X = 20
local BOX_POSITION_Y = 100
local BOX_WIDTH = 256 - BOX_POSITION_X * 2
local BOX_HEIGHT = 100

local DialogueBox = class("DialogueBox")

function DialogueBox:initialize(sprite, text)
    self.sprite = sprite
    self.text = text
    self.position = 1
    self.timer = 0
    self.isFinished = false
end

function DialogueBox:update()
    if milk.ticks > self.timer and not self.isFinished then
        self.position = self.position + 1
        self.timer = milk.ticks + TIMER_TICKS
        self.isFinished = self.position == #self.text
    end
end

function DialogueBox:draw()
    milk.rectfill(BOX_POSITION_X + 5, BOX_POSITION_Y, BOX_WIDTH, BOX_HEIGHT)
    milk.spr(self.sprite, BOX_POSITION_X + 10, BOX_POSITION_Y + 10)
    milk.sprfont(BOX_POSITION_X + 32, BOX_POSITION_Y + 10, string.sub(self.text, 1, self.position))
end

return DialogueBox
