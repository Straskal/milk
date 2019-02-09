local player = {
	speed = 2,
	flipped_x = false
 }

function player:begin()
	self.actor:set_animation('idle')
end

function player:update()
	if Input.get_key_pressed(Keys.F) then
		Window:toggle_fullscreen()
	end

	if Input.get_key_pressed(Keys.R) then
		Game:load_scene('res/scene.json')
	end

	local inputvec = Vector2D.new(0, 0)

	if Input.get_key(Keys.W) then
		inputvec.y = -1
	end
	if Input.get_key(Keys.S) then
		inputvec.y = 1
	end
	if Input.get_key(Keys.A) then
		inputvec.x = -1

		if (not self.flipped_x) then
			self.flipped_x = true
			self.actor:flip_x()
		end
	end
	if Input.get_key(Keys.D) then
		inputvec.x = 1

		if (self.flipped_x) then
			self.flipped_x = false
			self.actor:flip_x()
		end
	end

	if (inputvec == Vector2D.new(0, 0)) then
		self.actor:set_animation('idle')
	else
		self.actor:set_animation('run')
	end

	inputvec = inputvec * self.speed

	self.actor:move(inputvec.x, inputvec.y)
end

function player:on_collision(e)
	if e.other.name == 'door' then
		Game:load_scene('res/inside_scene.json')
	end
end

function player:destroyed()
end

function player:late_update()
	local clamped_cam_pos = Vector2D.new()
	clamped_cam_pos.x = Mathf.clamp(self.actor.position.x, 320, 320)
	clamped_cam_pos.y = Mathf.clamp(self.actor.position.y, 180, 360)

	self.scene:set_cam_pos(clamped_cam_pos)
end

return player