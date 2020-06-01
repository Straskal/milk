local milk = require("scripts.milk")

local stars = {}
local starColumns = {1,2,5,6,7,12}
local warpFactor = 3

local function init()
	milk.loadsnd(0, "sounds/music.wav")
	milk.loadsnd(1, "sounds/punch.wav")
	milk.loadsnd(2, "sounds/fireball_shoot.wav")

	for i = 1, #starColumns do
		for _ = 1, 10 do
			local star = {
				x = math.random(256),
				y = math.random(224),
				z = i,
				c = math.random(500000)
			}
			table.insert(stars, star)
		end
	end
end

local function update()
	for i = 1, #stars do
		local star = stars[i]
		star.y = star.y + star.z * warpFactor / 10
		if star.y > 224 then
			star.y = 0
			star.x = math.random(256)
		end
	end
end

local function draw()
	for i = 1, #stars do
		milk.pset(stars[i].x, stars[i].y, stars[i].c)
	end
end

return {
    init    	= init,
    update  	= update,
	draw	    = draw,
}