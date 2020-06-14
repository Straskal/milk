local milk = require "scripts.milk"
local class = require "scripts.class"

local STAR_COLUMNS = {1, 2, 5, 6, 7, 12}
local WARP_FACTOR = 3

local StarField = class("StarField")

function StarField:initialize()
    self:_createStars()
end

function StarField:_createStars()
    local stars = {}
    for i = 1, #STAR_COLUMNS do
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
    self.stars = stars
end

function StarField:update()
    for i = 1, #self.stars do
		local star = self.stars[i]
		star.y = star.y + star.z * WARP_FACTOR / 10
		if star.y > 224 then
			star.y = 0
			star.x = math.random(256)
		end
	end
end

function StarField:draw()
    for i = 1, #self.stars do
		milk.pset(self.stars[i].x, self.stars[i].y, self.stars[i].c)
	end
end

return StarField