# create test project
project(milktests)

# download and unpack googletest at configure time
#-------------------------------------------------------
configure_file(GTestDownload.txt googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
        )

if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
        )

if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL
        )

# create test exe
#-------------------------------------------------------
set (TESTS
        tests/math/Matrix3Tests.h
        tests/math/RectangleTests.h
        tests/scene/SceneTests.h
        )

set(MOCKS
        mocks/asset/MockTextureCache.h
        mocks/scene/MockActorComponent.h mocks/asset/MockActorLoader.h)

add_executable(milktests main.cpp ${TESTS} ${MOCKS})

target_include_directories(milktests
        PRIVATE
            ${milktests_SOURCE_DIR})

# link test exe with libs
target_link_libraries(milktests
        PRIVATE
            milklib
            gmock
            gtest
            gtest_main
        )

add_test(NAME milktests COMMAND milktests)