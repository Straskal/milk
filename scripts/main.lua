gTicks = 0;

gPlayer = {
	xPos 	= 10,
	yPos 	= 10,
	speed 	= 1,
	sprite 	= 0,
}

function _update()
	local mvx = 0
	local mvy = 0

	if btn(0) then mvy = -gPlayer.speed end
	if btn(1) then mvy = gPlayer.speed end
	if btn(2) then mvx = -gPlayer.speed end
	if btn(3) then mvx = gPlayer.speed end

	gPlayer.xPos = gPlayer.xPos + mvx
	gPlayer.yPos = gPlayer.yPos + mvy

	if mvx ~= 0 or mvy ~= 0 then
		if gTicks % 16 < 8 then
			gPlayer.sprite = 0
		else
			gPlayer.sprite = 1
		end
	end
end

function _draw()
	clrs(0x008751);
	spr(gPlayer.sprite, gPlayer.xPos, gPlayer.yPos);
	sprfont("Hey! You! C'mere!", 10, 40)
	gTicks = gTicks + 1
end
