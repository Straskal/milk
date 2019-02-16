cmake_minimum_required(VERSION 3.6.1)

project(milk)

# we're a c++14 family.
set(CMAKE_CXX_STANDARD 14)

# set debug symbol = _DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# set the ~/cmake module path for project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#---------------------------------------------------------------------------
# On windows, you have to point cmake to our third party dependencies.
#---------------------------------------------------------------------------
if (WIN32)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

        # provide paths for minGW/minGW-w64 development libs
        # set(SDL2_PATH ...)
        # set(SDL2_IMAGE_PATH ...)
        # set(ENV{LUA_DIR} ...)

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        # provide paths for visual c++ development libs
        set(SDL2_PATH C:/ExternalLibs/sdl2/SDL2-2.0.9)
        set(SDL2_IMAGE_PATH C:/ExternalLibs/SDL2_image-2.0.4)
        set(ENV{LUA_DIR} C:/ExternalLibs/lua53)

    endif()
endif()

#---------------------------------------------------------------------------
# run find scripts
#---------------------------------------------------------------------------
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Lua53 REQUIRED)

# not quite yet
# find_package(PhysFS REQUIRED)

#---------------------------------------------------------------------------
# code coverage configuration
#---------------------------------------------------------------------------
option(CODE_COVERAGE "Enable coverage reporting" OFF)

add_library(coverage_config INTERFACE)

if(CODE_COVERAGE)
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(coverage_config INTERFACE
            -O0        # no optimization
            -g         # generate debug info
            --coverage # sets all required flags
            )

    target_link_libraries(coverage_config INTERFACE --coverage)
endif()

#---------------------------------------------------------------------------
# enable testing up in this mafk
#---------------------------------------------------------------------------
include(CTest)
enable_testing()

# create milk lib
add_subdirectory(src)
# create milk exe
add_subdirectory(main)
# create test exe
add_subdirectory(test)

#---------------------------------------------------------------------------
# set start up project for visual studio
#---------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT milk)