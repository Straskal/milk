# You must download dependencies. If on Windows, then you have to manually set paths to dependencies. See line 20
# SDL2: https://www.libsdl.org/download-2.0.php
# SDL2_image: https://www.libsdl.org/projects/SDL_image/
# Lua 5.3: https://www.lua.org/download.html which leads to https://sourceforge.net/projects/luabinaries/files/5.3.5/

cmake_minimum_required(VERSION 3.6.1)

project(milk)

# we're a c++14 family.
set(CMAKE_CXX_STANDARD 14)

# set debug symbol = _DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# set the ~/cmake module path for project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#---------------------------------------------------------------------------
# On windows, we have to point cmake to our third party dependencies.
#---------------------------------------------------------------------------

if(WIN32)
	if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		# provide paths for minGW/minGW-w64 development libs
		# set(SDL2_PATH ...)
		# set(SDL2_IMAGE_PATH ...)
		# set(ENV{LUA_DIR} ...)
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		# provide paths for visual c++ development libs
		# these are just the paths that I use. Feel free to change them in your environment.
		set(SDL2_PATH C:/ExternalLibs/SDL2-2.0.9)
		set(SDL2_IMAGE_PATH C:/ExternalLibs/SDL2_image-2.0.4)
		set(ENV{LUA_DIR} C:/ExternalLibs/lua53)
	endif()
endif()

#---------------------------------------------------------------------------
# find dependencies
#---------------------------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Lua53 REQUIRED)

#---------------------------------------------------------------------------
# milk lib
#---------------------------------------------------------------------------

# enable testing for milk lib
include(CTest)	
enable_testing()

# common
set(MILK_SRC_DATA
	src/data/int.h
)

# input
set(MILK_SRC_INPUT
	src/input/Keyboard.cpp
	src/input/Keyboard.h
)

# graphics
set(MILK_SRC_GRAPHICS
	src/graphics/Color.h
	src/graphics/Texture.cpp
	src/graphics/Texture.h
)

# math
set(MILK_SRC_MAIN
	src/main/Locator.cpp
	src/main/Locator.h
	src/main/MilkState.cpp
	src/main/MilkState.h
)

# math
set(MILK_SRC_MATH
	src/math/Matrix3.h
	src/math/Rectangle.h
	src/math/Vector2.h
)

# node
set(MILK_SRC_NODE
	src/node/node_id.cpp
	src/node/node_id.h
	src/node/NodeTree.cpp
	src/node/NodeTree.h
)

# script
set(MILK_SRC_SCRIPT
	src/script/LuaApi.cpp
	src/script/LuaApi.h
	src/script/lua_extensions.cpp
	src/script/lua_extensions.h
)

# window
set(MILK_SRC_WINDOW
	src/window/Renderer.h
	src/window/Window.h
)

set(MILK_SRC_WINDOW_SDL
	src/window/sdl/Renderer.cpp
	src/window/sdl/Renderer.h
	src/window/sdl/Window.cpp
	src/window/sdl/Window.h
)

set(MILK_SRC_FILES
	${MILK_SRC_DATA}
	${MILK_SRC_INPUT}
	${MILK_SRC_GRAPHICS}
	${MILK_SRC_SCRIPT}
	${MILK_SRC_MAIN}
	${MILK_SRC_MATH}
	${MILK_SRC_NODE}
	${MILK_SRC_WINDOW}
	${MILK_SRC_WINDOW_SDL}
)

add_library(milklib ${MILK_SRC_FILES})

target_include_directories(milklib
	PUBLIC
		${SDL2_INCLUDE_DIR}
		${SDL2_IMAGE_INCLUDE_DIRS}
		${LUA53_INCLUDE_DIR}
		src
		libs
)

if (WIN32)
	target_link_directories(milklib
		PUBLIC
			${SDL2_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
			${LUA53_INCLUDE_DIR}
			src
			libs
	)
endif()

target_link_libraries(milklib
	PUBLIC
		${SDL2_LIBRARY}
		${SDL2_IMAGE_LIBRARIES}
		${LUA53_LIBRARIES}
)

#---------------------------------------------------------------------------
# milk exe
#---------------------------------------------------------------------------

add_executable(milk src/main/main.cpp)

target_link_libraries(milk milklib)

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${milk_BINARY_DIR})

#---------------------------------------------------------------------------
# add milk lib tests
#---------------------------------------------------------------------------

# download and unpack googletest at configure time
configure_file(test/GTestDownload.txt googletest-download/CMakeLists.txt)

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
set (TESTS
	test/math/Matrix3Tests.h
	test/math/RectangleTests.h
	test/node/NodeIdTests.h
	test/node/NodeTreeTests.h
	test/window/MockRenderer.h
	test/window/MockRenderer.h
)

set(MOCKS
)

add_executable(milktests test/main.cpp ${TESTS} ${MOCKS})

target_include_directories(milktests
	PRIVATE
		test
)

# link test exe with libs
target_link_libraries(milktests
	PRIVATE
		milklib
		gmock
		gtest
		gtest_main
)

add_test(NAME milktests COMMAND milktests)

#---------------------------------------------------------------------------
# set start up project for visual studio
#---------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT milk)
endif()

#---------------------------------------------------------------------------
# post build step to copy dlls to executable output file
# currently only set up for visual c++ x86 dlls on windows

# assumes SDL2 2.0.9
# assumes SDL2_image 2.0.4
# assumes Lua 5.3.5
#---------------------------------------------------------------------------

if (WIN32)
	add_custom_command(TARGET milk POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${PROJECT_SOURCE_DIR}/libs"
			$<TARGET_FILE_DIR:milk>)
endif()
