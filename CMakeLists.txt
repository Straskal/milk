cmake_minimum_required(VERSION 3.12.4)

project(libmilk)
set(C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(MILK_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
endif()

# SDL2: 	https://www.libsdl.org/download-2.0.php
# Lua 5.3: 	https://sourceforge.net/projects/luabinaries/files/5.3.5/
if (WIN32)
	if (MINGW)
		# provide paths for minGW/minGW-w64 development libs
		set(SDL2_PATH C:/ExternalLibs/SDL2-devel-2.0.10-mingw/i686-w64-mingw32)
		set(ENV{LUA_DIR} C:/ExternalLibs/Lua53mingw)
	elseif (MSVC)
		# provide paths for visual c++ development libs
		set(SDL2_PATH C:/ExternalLibs/SDL2-2.0.10)
		set(ENV{LUA_DIR} C:/ExternalLibs/lua53)
	endif()
endif()

find_package(SDL2 REQUIRED)
find_package(Lua53 REQUIRED)

set(MILK_SRC_FILES
	src/audio.c
	src/audio.h
	src/bitmap.c
	src/bitmap.h
	src/common.h
	src/gamepad.c
	src/gamepad.h
	src/logs.c
	src/logs.h
	src/platform.h
	src/wave.c
	src/wave.h
	src/video.c
	src/video.h
	src/embed/font.inl
)

set(MILK_MAIN_SRC_FILES
	src/console.c
	src/console.h
	src/milk.c
	src/milk.h
	src/platform/sdlmain.c
)

add_executable(milk ${MILK_SRC_FILES} ${MILK_MAIN_SRC_FILES})
target_compile_definitions(milk PUBLIC BUILD_WITH_CONSOLE)
target_include_directories(milk PUBLIC ${SDL2_INCLUDE_DIR} ${LUA53_INCLUDE_DIR} ${MILK_SRC_DIR})
if (WIN32)
	target_link_directories(milk PUBLIC ${SDL2_INCLUDE_DIR} ${LUA53_INCLUDE_DIR} ${MILK_SRC_DIR})
endif()
target_link_libraries(milk PUBLIC ${SDL2_LIBRARY} ${LUA53_LIBRARIES})

if(WIN32)
	if (MSVC)
		set(MILK_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/msvc-x86)
	elseif (MINGW)
		set(MILK_LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/mingw-w64-i686)
	endif()

	add_custom_command(TARGET milk POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${MILK_LIBS_DIR}
		$<TARGET_FILE_DIR:milk>
	)
endif()
