local function createEnemies(self)
    local currentWave = self.waves[self.currentWave]
    
end

local function init(self, waves)
    self.waves = waves
    self.currentWave = 0
end

local function update(self)
end

local function draw(self)
end

return {
    init = init,
    update = update,
    draw = draw
}