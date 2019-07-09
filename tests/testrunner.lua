local tests = {
    DRAW_TEST = "tests/drawtest.lua"
}

local answer
repeat
    io.write("Enter test name: ")
    io.flush()
    answer = io.read()
    if tests[answer] then
        dofile(tests[answer])
    end
until answer == "STOP"