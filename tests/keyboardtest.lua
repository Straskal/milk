local window = require("milk.window")
local keyboard = require("milk.keyboard")
local keys = keyboard.keys

--[[
    the draw test proves the following:
    - that the keyboard works lol
--]]
local test = {}

function test.tick()
    if keyboard.is_key_released(keys.A) then
        print("A was hit")
    end
    if keyboard.is_key_released(keys.B) then
        print("B was hit")
    end
    if keyboard.is_key_released(keys.C) then
        print("C was hit")
    end
    if keyboard.is_key_released(keys.D) then
        print("D was hit")
    end
    if keyboard.is_key_released(keys.E) then
        print("E was hit")
    end
    if keyboard.is_key_released(keys.F) then
        print("F was hit")
    end
    if keyboard.is_key_released(keys.G) then
        print("G was hit")
    end
    if keyboard.is_key_released(keys.H) then
        print("H was hit")
    end
    if keyboard.is_key_released(keys.I) then
        print("I was hit")
    end
    if keyboard.is_key_released(keys.J) then
        print("J was hit")
    end
    if keyboard.is_key_released(keys.K) then
        print("K was hit")
    end
    if keyboard.is_key_released(keys.L) then
        print("L was hit")
    end
    if keyboard.is_key_released(keys.M) then
        print("M was hit")
    end
    if keyboard.is_key_released(keys.N) then
        print("N was hit")
    end
    if keyboard.is_key_released(keys.O) then
        print("O was hit")
    end
    if keyboard.is_key_released(keys.P) then
        print("P was hit")
    end
    if keyboard.is_key_released(keys.Q) then
        print("Q was hit")
    end
    if keyboard.is_key_released(keys.R) then
        print("R was hit")
    end
    if keyboard.is_key_released(keys.S) then
        print("S was hit")
    end
    if keyboard.is_key_released(keys.T) then
        print("T was hit")
    end
    if keyboard.is_key_released(keys.U) then
        print("U was hit")
    end
    if keyboard.is_key_released(keys.V) then
        print("V was hit")
    end
    if keyboard.is_key_released(keys.W) then
        print("W was hit")
    end
    if keyboard.is_key_released(keys.X) then
        print("X was hit")
    end
    if keyboard.is_key_released(keys.Y) then
        print("Y was hit")
    end
    if keyboard.is_key_released(keys.Z) then
        print("Z was hit")
    end
    if keyboard.is_key_released(keys.NUM1) then
        print("1 was hit")
    end
    if keyboard.is_key_released(keys.NUM2) then
        print("2 was hit")
    end
    if keyboard.is_key_released(keys.NUM3) then
        print("3 was hit")
    end
    if keyboard.is_key_released(keys.NUM4) then
        print("4 was hit")
    end
    if keyboard.is_key_released(keys.NUM5) then
        print("5 was hit")
    end
    if keyboard.is_key_released(keys.NUM6) then
        print("6 was hit")
    end
    if keyboard.is_key_released(keys.NUM7) then
        print("7 was hit")
    end
    if keyboard.is_key_released(keys.NUM8) then
        print("8 was hit")
    end
    if keyboard.is_key_released(keys.NUM9) then
        print("9 was hit")
    end
    if keyboard.is_key_released(keys.NUM0) then
        print("0 was hit")
    end
    if keyboard.is_key_released(keys.RETURN) then
        print("RETURN was hit")
    end
    if keyboard.is_key_released(keys.ESCAPE) then
        print("ESCAPE was hit")
        window.close()
    end
    if keyboard.is_key_released(keys.BACKSPACE) then
        print("BACKSPACE was hit")
    end
    if keyboard.is_key_released(keys.TAB) then
        print("TAB was hit")
    end
    if keyboard.is_key_released(keys.SPACE) then
        print("SPACEBAR was hit")
    end
    if keyboard.is_key_released(keys.MINUS) then
        print("MINUS was hit")
    end
    if keyboard.is_key_released(keys.EQUALS) then
        print("EQUALS was hit")
    end
    if keyboard.is_key_released(keys.LEFTBRACKET) then
        print("LEFTBRACKET was hit")
    end
    if keyboard.is_key_released(keys.RIGHTBRACKET) then
        print("RIGHTBRACKET was hit")
    end
    if keyboard.is_key_released(keys.BACKSLASH) then
        print("BACKSLASH was hit")
    end
    if keyboard.is_key_released(keys.SEMICOLON) then
        print("SEMICOLON was hit")
    end
    if keyboard.is_key_released(keys.APOSTROPHE) then
        print("APOSTROPHE was hit")
    end
    if keyboard.is_key_released(keys.TILDE) then
        print("TILDE was hit")
    end
    if keyboard.is_key_released(keys.COMMA) then
        print("COMMA was hit")
    end
    if keyboard.is_key_released(keys.SLASH) then
        print("SLASH was hit")
    end
    if keyboard.is_key_released(keys.CAPSLOCK) then
        print("CAPSLOCK was hit")
    end
    if keyboard.is_key_released(keys.F1) then
        print("F1 was hit")
    end
    if keyboard.is_key_released(keys.F2) then
        print("F2 was hit")
    end
    if keyboard.is_key_released(keys.F3) then
        print("F3 was hit")
    end
    if keyboard.is_key_released(keys.F4) then
        print("F4 was hit")
    end
    if keyboard.is_key_released(keys.F5) then
        print("F5 was hit")
    end
    if keyboard.is_key_released(keys.F6) then
        print("F6 was hit")
    end
    if keyboard.is_key_released(keys.F7) then
        print("F7 was hit")
    end
    if keyboard.is_key_released(keys.F8) then
        print("F8 was hit")
    end
    if keyboard.is_key_released(keys.F9) then
        print("F9 was hit")
    end
    if keyboard.is_key_released(keys.F10) then
        print("F10 was hit")
    end
    if keyboard.is_key_released(keys.F11) then
        print("F11 was hit")
    end
    if keyboard.is_key_released(keys.F12) then
        print("F12 was hit")
    end
    if keyboard.is_key_released(keys.DELETE) then
        print("A was hit")
    end
    if keyboard.is_key_released(keys.RIGHT) then
        print("DELETE was hit")
    end
    if keyboard.is_key_released(keys.LEFT) then
        print("LEFT was hit")
    end
    if keyboard.is_key_released(keys.DOWN) then
        print("DOWN was hit")
    end
    if keyboard.is_key_released(keys.UP) then
        print("UP was hit")
    end
    if keyboard.is_key_released(keys.LCTRL) then
        print("LECT CONTROL was hit")
    end
    if keyboard.is_key_released(keys.LSHIFT) then
        print("LEFT SHIFT was hit")
    end
    if keyboard.is_key_released(keys.LALT) then
        print("LEFT ALT was hit")
    end
    if keyboard.is_key_released(keys.RCTRL) then
        print("RIGHT CONTROL was hit")
    end
    if keyboard.is_key_released(keys.RSHIFT) then
        print("RIGHT SHIFT was hit")
    end
end

return test
