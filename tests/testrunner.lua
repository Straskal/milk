local tests = {
    WINDOW_TEST = "tests/windowtest.lua",
    DRAW_TEST = "tests/drawtest.lua"
}

local answer
repeat
    io.write("Enter test name: ")
    io.flush()
    answer = io.read()
    if answer == "RUN_ALL" then
        for _, testfile in pairs(tests) do
            dofile(testfile)
        end
    elseif tests[answer] then
        dofile(tests[answer])
    end
until answer == "STOP"