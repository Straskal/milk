local ticks = 0;

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

		self.xPos = self.xPos + mvx
		self.yPos = self.yPos + mvy
	
		if mvx ~= 0 or mvy ~= 0 then
			if ticks % 16 < 8 then
				self.sprite = 0
			else
				self.sprite = 1
			end
		end
	end,
	draw = function(self) 
		spr(self.sprite, self.xPos, self.yPos)
	end
}

function _init()
	-- initialization
end

function _update()
	player:update()
end

function _draw()
	clrs(0)
	player:draw()
	ticks = ticks + 1
end
