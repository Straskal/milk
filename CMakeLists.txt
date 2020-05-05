# You must download dependencies. If on Windows, then you have to manually set paths to dependencies. See line 20
# SDL2: https://www.libsdl.org/download-2.0.php
# SDL2_image: https://www.libsdl.org/projects/SDL_image/
# Lua 5.3: https://www.lua.org/download.html which leads to https://sourceforge.net/projects/luabinaries/files/5.3.5/

cmake_minimum_required(VERSION 3.6.3)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(libmilk)

set(C_STANDARD 11)

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
		set(ENV{LUA_DIR} C:/ExternalLibs/Lua53mingw)
	elseif (MSVC)
		# provide paths for visual c++ development libs
		# these are just the paths that I use. Feel free to change them in your environment.
		set(SDL2_PATH C:/ExternalLibs/SDL2-2.0.10)
		set(ENV{LUA_DIR} C:/ExternalLibs/lua53)
	endif()
endif()

#---------------------------------------------------------------------------
# find dependencies
#---------------------------------------------------------------------------

find_package(SDL2 2.0.10 EXACT REQUIRED)

#---------------------------------------------------------------------------
# milk lib
#---------------------------------------------------------------------------

set(MILK_SRC_FILES
	src/main.c
	src/milk.c
	src/milk.h
	src/milk_api.c
	src/milk_api.h
	src/sprite.c
	src/sprite.h
)

add_executable(milk ${MILK_SRC_FILES})

target_include_directories(milk
	PUBLIC
		${SDL2_INCLUDE_DIR}
		${LUA53_INCLUDE_DIR}
		${MILK_SRC_DIR}
)

if (WIN32)
	target_link_directories(milk
		PUBLIC
			${SDL2_INCLUDE_DIR}
			${LUA53_INCLUDE_DIR}
			${MILK_SRC_DIR}
	)
endif()

target_link_libraries(milk
	PUBLIC
		${SDL2_LIBRARY}
		${LUA53_LIBRARIES}
)

#---------------------------------------------------------------------------
# post build step to copy dlls to output file

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
