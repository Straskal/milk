local milk = require("scripts.milk")

local introState = {}

function introState.update()
end

function introState.draw()
    milk.sprfont(10, 10, "this is the intro state.")
end

return introState