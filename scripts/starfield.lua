local ticks = 0;
local stars = {}
local starColumns = {1,2,5,6,7,12}
local warpFactor = 3

local player = {
	xPos 		= 10,
	yPos 		= 10,
	speed 		= 1,
	sprite 		= 0,
	update = function(self)
		local mvx = 0
		local mvy = 0

		if btn(0) then mvy = -self.speed end
		if btn(1) then mvy = self.speed end
		if btn(2) then mvx = -self.speed end
		if btn(3) then mvx = self.speed end

		if btnp(4) then snd(1) end
		if btnp(5) then snd(2) end
		if btnp(6) then snd(0, 128, 1) end

		self.xPos = self.xPos + mvx
		self.yPos = self.yPos + mvy
	
		if ticks % 24 < 12 then
			self.sprite = 4
		else
			self.sprite = 5
		end
	end,
	draw = function(self) 
		spr(self.sprite, self.xPos, self.yPos)
	end
}


local function init()
	for i = 1, #starColumns do
		for j = 1, 10 do
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
	player:update()
end


local function draw()	
	clrs(0x1a1a1a)

	for i = 1, #stars do
		local star = stars[i]
		star.y = star.y + star.z * warpFactor / 10		
		if star.y > 224 then
			star.y = 0
			star.x = math.random(256)
		end
		pset(stars[i].x, stars[i].y, stars[i].c)
	end

	player:draw()
	ticks = ticks + 1
end

return {
    init    = init,
    update  = update,
    draw    = draw
}