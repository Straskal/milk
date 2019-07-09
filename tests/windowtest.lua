local milk = require("milk")
local window = require("milk.window")
local time = require("milk.time")

--[[
    the window test proves the following:
    - the window fullscreen toggle works
	- window sizing works
--]]
milk.init()

window.show()
print("window shown")
print("window will enter fullscreen in 5 seconds...")
time.delay(5)
window.set_fullscreen(true)
print("window is now in fullscreen mode")
print("window will exit fullscreen in 5 seconds...")
time.delay(5)
window.set_fullscreen(false)
print("window is now in windowed mode")
print("window will change from 800x600 to 300x300 in 3 seconds...")
time.delay(3)
window.set_size(300, 300)
print("window is now 300x300")
print("test finished and will end in 3 seconds...")
time.delay(3)

milk.quit()