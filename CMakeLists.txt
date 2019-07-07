# You must download dependencies. If on Windows, then you have to manually set paths to dependencies. See line 20
# SDL2: https://www.libsdl.org/download-2.0.php
# SDL2_image: https://www.libsdl.org/projects/SDL_image/
# Lua 5.3: https://www.lua.org/download.html which leads to https://sourceforge.net/projects/luabinaries/files/5.3.5/

cmake_minimum_required(VERSION 3.6.3)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(libmilk)

# we're a c++11 family
set(CMAKE_CXX_STANDARD 11)

# set debug symbol = MILK_DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMILK_DEBUG")

# give is warning hell, brother
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# set the ~/cmake module path for project
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(MILK_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#---------------------------------------------------------------------------
# On windows, we have to point cmake to our third party dependencies.
#---------------------------------------------------------------------------

if(WIN32)
	if (MINGW)
		# provide paths for minGW/minGW-w64 development libs
		set(SDL2_PATH C:/ExternalLibs/SDL2-devel-2.0.10-mingw/SDL2-2.0.10/i686-w64-mingw32)
		set(SDL2_IMAGE_PATH C:/ExternalLibs/SDL2_image-devel-2.0.4-mingw/SDL2_image-2.0.4/i686-w64-mingw32)
		set(SDL2_MIXER_PATH C:/ExternalLibs/SDL2_mixer-devel-2.0.4-mingw/SDL2_mixer-2.0.4/i686-w64-mingw32)
		set(ENV{LUA_DIR} C:/ExternalLibs/Lua53mingw)
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

# root
set(MILK_SRC
	src/luamlib.cpp
	src/luamlib.h
	src/milk_export.h
	src/State.cpp
	src/State.h
	src/luaopen_milk.cpp
	src/luaopen_milk.h
)

# audio
set(MILK_SRC_AUDIO
	src/audio/AudioPlayer.h
	src/audio/Music.h
	src/audio/MusicCache.h
	src/audio/SampleState.h
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

# data
set(MILK_SRC_DATA
	src/data/uid.cpp
	src/data/uid.h
	src/data/int.h
)

# graphics
set(MILK_SRC_GRAPHICS
	src/graphics/graphics.cpp
	src/graphics/graphics.h
	src/graphics/image.h
	src/graphics/luaopen_milk_graphics.cpp
	src/graphics/luaopen_milk_graphics.h
)

# keyboard
set(MILK_SRC_KEYBOARD
	src/keyboard/Keyboard.h
	src/keyboard/luaopen_milk_keyboard.cpp
	src/keyboard/luaopen_milk_keyboard.h
	src/keyboard/sdl/SDLKeyboard.cpp
	src/keyboard/sdl/SDLKeyboard.h
)

# mouse
set(MILK_SRC_MOUSE
	src/mouse/Mouse.h
	src/mouse/luaopen_milk_mouse.cpp
	src/mouse/luaopen_milk_mouse.h
	src/mouse/sdl/SDLMouse.cpp
	src/mouse/sdl/SDLMouse.h
)

# time
set(MILK_SRC_TIME
	src/time/Time.h
	src/time/luaopen_milk_time.cpp
	src/time/luaopen_milk_time.h
	src/time/sdl/SDLTime.cpp
	src/time/sdl/SDLTime.h
)

# window
set(MILK_SRC_WINDOW
	src/window/window.cpp
	src/window/window.h	
	src/window/luaopen_milk_window.cpp
	src/window/luaopen_milk_window.h
)

set(MILK_SRC_FILES
	${MILK_SRC}
	${MILK_SRC_AUDIO}
	${MILK_SRC_DATA}
	${MILK_SRC_CORE}
	${MILK_SRC_GRAPHICS}
	${MILK_SRC_KEYBOARD}
	${MILK_SRC_MOUSE}
	${MILK_SRC_TIME}
	${MILK_SRC_WINDOW}
)

add_library(milk SHARED ${MILK_SRC_FILES})

target_include_directories(milk
	PUBLIC
		${SDL2_INCLUDE_DIR}
		${SDL2_IMAGE_INCLUDE_DIRS}
		${SDL2_MIXER_INCLUDE_DIRS}
		${LUA53_INCLUDE_DIR}
		${MILK_SRC_DIR}
)

if (WIN32)
	target_link_directories(milk
		PUBLIC
			${SDL2_INCLUDE_DIR}
			${SDL2_IMAGE_INCLUDE_DIRS}
			${SDL2_MIXER_INCLUDE_DIRS}
			${LUA53_INCLUDE_DIR}
			${MILK_SRC_DIR}
	)
endif()

target_link_libraries(milk
	PUBLIC
		${SDL2_LIBRARY}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_MIXER_LIBRARIES}
		${LUA53_LIBRARIES}
)

#---------------------------------------------------------------------------
# post build step to copy dlls to executable output file

# assumes SDL2 2.0.10
# assumes SDL2_image 2.0.4
# assumes Lua 5.3.5
#---------------------------------------------------------------------------

if(WIN32)
	if (MSVC)
		set(MILK_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/msvc-x86)
	elseif (MINGW)
		set(MILK_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/mingw-w64-i686)
	endif()

	add_custom_command(TARGET milk POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${MILK_LIBS_DIR}
			$<TARGET_FILE_DIR:milk>)
endif()
