local milk = {}

milk.btn        = _G.btn
milk.btnp       = _G.btnp
milk.clip       = _G.clip
milk.clrs       = _G.clrs
milk.pset       = _G.pset
milk.rect       = _G.rect
milk.rectfill   = _G.rectfill
milk.spr        = _G.spr
milk.sprfont    = _G.sprfont
milk.loadsnd    = _G.loadsnd
milk.snd        = _G.snd
milk.vol        = _G.vol

function _G._init()
    if milk.init then milk.init() end
end

function _G._update()
    if milk.update then milk.update() end
end

function _G._draw()
    if milk.draw then milk.draw() end
end

return milk