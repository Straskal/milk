local tests = {
    WINDOW_TEST = "tests/windowtest.lua",
    DRAW_TEST = "tests/drawtest.lua"
}

local function run_test(name, file)
    print("=================================")
    print("RUNNING " .. name)
    print("=================================")
    dofile(file)
end

local test_to_run
repeat
    print("TEST RUNNER")
    print("=================================")
    io.write("Enter test name: ")
    io.flush()
    test_to_run = io.read()
    if test_to_run == "RUN_ALL" then
        for test_name, test_file in pairs(tests) do
            run_test(test_name, test_file)
        end
    elseif tests[test_to_run] then
        run_test(test_to_run, tests[test_to_run])
    end
until test_to_run == "STOP"