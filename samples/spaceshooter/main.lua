local milk = require "scripts.milk"
local game = require "scripts.game"

-- if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
-- 	require("lldebugger").start(false)
-- end

function milk.init()
	game.init()
end

function milk.update()
	game.update()
end

function milk.draw()
	game.draw()
end
