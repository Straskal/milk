-- Globals are typically frowned upon and static analyzers will scream, milk.lua to the rescue.
-- milk.lua just encapsulates milk's API to follow best practices and can be required from other scripts.

local milk = {}

milk.ticks = 0

-- Constants
milk.btnstart   = 0
milk.btnup      = 1
milk.btndown    = 2
milk.btnleft    = 3
milk.btnright   = 4
milk.btna       = 5
milk.btnb       = 6
milk.btnx       = 7
milk.btny       = 8

-- API
milk.btn        = _G.btn
milk.btnp       = _G.btnp
milk.clip       = _G.clip
milk.clrs       = _G.clrs
milk.pset       = _G.pset
milk.rect       = _G.rect
milk.rectfill   = _G.rectfill
milk.spr        = _G.spr
milk.sprfont    = _G.sprfont
milk.allocsnd   = _G.allocsnd
milk.freesnd    = _G.freesnd
milk.play       = _G.play
milk.stop       = _G.stop
milk.pause      = _G.pause
milk.resume     = _G.resume
milk.sndslot    = _G.sndslot
milk.vol        = _G.vol

function _G._init()
    if milk.init then milk.init() end
end

function _G._update()
    if milk.update then milk.update() end
end

function _G._draw()
    if milk.draw then milk.draw() end
    milk.ticks = milk.ticks + 1
end

return milk