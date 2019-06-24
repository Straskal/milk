# You must download dependencies. If on Windows, then you have to manually set paths to dependencies. See line 20
# SDL2: https://www.libsdl.org/download-2.0.php
# SDL2_image: https://www.libsdl.org/projects/SDL_image/
# Lua 5.3: https://www.lua.org/download.html which leads to https://sourceforge.net/projects/luabinaries/files/5.3.5/

cmake_minimum_required(VERSION 3.6.3)

project(milk)

# we're a c++14 family
set(CMAKE_CXX_STANDARD 14)

# set debug symbol = MILK_DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMILK_DEBUG")

# set the ~/cmake module path for project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(MILK_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MILKTESTS_SRC_DIR ${CMAKE_SOURCE_DIR}/test)
set(MILK_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(MILK_RESOURCES_DIR ${CMAKE_SOURCE_DIR}/res)
endif()

include(CTest)

#---------------------------------------------------------------------------
# On windows, we have to point cmake to our third party dependencies.
#---------------------------------------------------------------------------

if(WIN32)
	if (MINGW)
		# provide paths for minGW/minGW-w64 development libs
		# set(SDL2_PATH ...)
		# set(SDL2_IMAGE_PATH ...)
		# set(SDL2_MIXER_PATH ...)
		# set(ENV{LUA_DIR} ...)
	elseif (MSVC)
		# provide paths for visual c++ development libs
		# these are just the paths that I use. Feel free to change them in your environment.
		set(SDL2_PATH C:/ExternalLibs/SDL2-2.0.10)
		set(SDL2_IMAGE_PATH C:/ExternalLibs/SDL2_image-2.0.4)
		set(SDL2_MIXER_PATH C:/ExternalLibs/SDL2_mixer-2.0.4)
		set(ENV{LUA_DIR} C:/ExternalLibs/lua53)
	endif()
endif()

#---------------------------------------------------------------------------
# find dependencies
#---------------------------------------------------------------------------

find_package(SDL2 2.0.10 EXACT REQUIRED)
find_package(SDL2_image 2.0.4 EXACT REQUIRED)
find_package(SDL2_mixer 2.0.4 EXACT REQUIRED)
find_package(Lua53 REQUIRED)

#---------------------------------------------------------------------------
# milk lib
#---------------------------------------------------------------------------

# audio
set(MILK_SRC_AUDIO
	src/audio/AudioPlayer.h
	src/audio/Music.h
	src/audio/MusicCache.h
	src/audio/SoundCache.h
	src/audio/Sound.h
	src/audio/luaopen_milk_audio.cpp
	src/audio/luaopen_milk_audio.h
	src/audio/sdl/SDLAudioPlayer.cpp
	src/audio/sdl/SDLAudioPlayer.h
	src/audio/sdl/SDLMusicCache.cpp
	src/audio/sdl/SDLMusicCache.h
	src/audio/sdl/SDLSoundCache.cpp
	src/audio/sdl/SDLSoundCache.h
)

# core
set(MILK_SRC_CORE
	src/core/Locator.cpp
	src/core/Locator.h
	src/core/luamlib.cpp
	src/core/luamlib.h
	src/core/run.cpp
	src/core/run.h
)

# data
set(MILK_SRC_DATA
	src/data/int.h
)

# graphics
set(MILK_SRC_GRAPHICS
	src/graphics/Color.h
	src/graphics/Renderer.h
	src/graphics/Texture.h
	src/graphics/TextureCache.h
	src/graphics/luaopen_milk_graphics.cpp
	src/graphics/luaopen_milk_graphics.h
	src/graphics/sdl/SDLRenderer.cpp
	src/graphics/sdl/SDLRenderer.h
	src/graphics/sdl/SDLTextureCache.cpp
	src/graphics/sdl/SDLTextureCache.h
)

# keyboard
set(MILK_SRC_KEYBOARD
	src/keyboard/Keyboard.h
	src/keyboard/luaopen_milk_keyboard.cpp
	src/keyboard/luaopen_milk_keyboard.h
	src/keyboard/sdl/SDLKeyboard.cpp
	src/keyboard/sdl/SDLKeyboard.h
)

# math
set(MILK_SRC_MATH
	src/math/Matrix3.h
	src/math/Rectangle.h
	src/math/Vector2.h
)

# mouse
set(MILK_SRC_MOUSE
	src/mouse/Mouse.h
	src/mouse/luaopen_milk_mouse.cpp
	src/mouse/luaopen_milk_mouse.h
	src/mouse/sdl/SDLMouse.cpp
	src/mouse/sdl/SDLMouse.h
)

# window
set(MILK_SRC_WINDOW
	src/window/Window.h	
	src/window/luaopen_milk_window.cpp
	src/window/luaopen_milk_window.h
	src/window/sdl/SDLWindow.cpp
	src/window/sdl/SDLWindow.h
)

set(MILK_SRC_FILES
	${MILK_SRC_AUDIO}
	${MILK_SRC_CORE}
	${MILK_SRC_DATA}
	${MILK_SRC_GRAPHICS}
	${MILK_SRC_KEYBOARD}
	${MILK_SRC_MATH}
	${MILK_SRC_MOUSE}
	${MILK_SRC_WINDOW}
)

add_library(milklib STATIC ${MILK_SRC_FILES})

target_include_directories(milklib
	PUBLIC
		${SDL2_INCLUDE_DIR}
		${SDL2_IMAGE_INCLUDE_DIRS}
		${SDL2_MIXER_INCLUDE_DIRS}
		${LUA53_INCLUDE_DIR}
		${MILK_SRC_DIR}
)

if (WIN32)
	target_link_directories(milklib
		PUBLIC
			${SDL2_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
			${SDL2_MIXER_INCLUDE_DIRS}
			${LUA53_INCLUDE_DIR}
			${MILK_SRC_DIR}
	)
endif()

target_link_libraries(milklib
	PUBLIC
		${SDL2_LIBRARY}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_MIXER_LIBRARIES}
		${LUA53_LIBRARIES}
)

target_compile_definitions(milklib
	PRIVATE
		MILK_MAIN_LUA_PATH="res/main.lua"
)

#---------------------------------------------------------------------------
# milk exe
#---------------------------------------------------------------------------

add_executable(milk src/main.cpp)

target_link_libraries(milk milklib)

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
			${MILK_LIBS_DIR}
			$<TARGET_FILE_DIR:milk>)
endif()
