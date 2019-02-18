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
if (WIN32)

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

        # provide paths for minGW/minGW-w64 development libs
        # set(SDL2_PATH ...)
        # set(SDL2_IMAGE_PATH ...)
        # set(ENV{LUA_DIR} ...)

    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

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

#---------------------------------------------------------------------------
# milk lib
#---------------------------------------------------------------------------

set(MILK_SRC_LIBS
    libs/json/json.hpp
    libs/sol/sol.hpp
)

source_group("libs" FILES ${MILK_SRC_LIBS})

set(MILK_SRC_ASSET
    src/asset/AssetCache.h
    src/asset/ActorLoader.h
    src/asset/SceneLoader.h
    src/asset/adapter/ActorLoaderAdapter.cpp
    src/asset/adapter/ActorLoaderAdapter.h
    src/asset/adapter/ActorJsonDeserializer.h
    src/asset/adapter/ActorTemplateCacheAdapter.cpp
    src/asset/adapter/ActorTemplateCacheAdapter.h
    src/asset/adapter/SceneLoaderAdapter.cpp
    src/asset/adapter/SceneLoaderAdapter.h
    src/asset/adapter/SceneJsonDeserializer.h
    src/asset/adapter/TextureCacheAdapter.cpp
    src/asset/adapter/TextureCacheAdapter.h
    src/asset/adapter/deserializers-v1/ActorJsonDeserializerV1.cpp
    src/asset/adapter/deserializers-v1/ActorJsonDeserializerV1.h
    src/asset/adapter/deserializers-v1/SceneJsonDeserializerV1.cpp
    src/asset/adapter/deserializers-v1/SceneJsonDeserializerV1.h
)

source_group("asset" FILES ${MILK_SRC_ASSET})

set(MILK_SRC_DEBUG_TOOLS
    src/debugtools/DebugTools.cpp
    src/debugtools/DebugTools.h
)

source_group("debugtools" FILES ${MILK_SRC_DEBUG_TOOLS})

set(MILK_SRC_FILESYSTEM
    src/filesystem/Filesystem.h
    src/filesystem/adapter/FilesystemAdapter.cpp
    src/filesystem/adapter/FilesystemAdapter.h
)

source_group("filesystem" FILES ${MILK_SRC_FILESYSTEM})

set(MILK_SRC_INPUT
    src/input/Keyboard.cpp
    src/input/Keyboard.h
    src/input/binding/LuaKeyboard.cpp
    src/input/binding/LuaKeyboard.h
)

source_group("input" FILES ${MILK_SRC_INPUT})

set(MILK_SRC_GAME
    src/game/Game.h
    src/game/Game.cpp
    src/game/binding/LuaGame.cpp
    src/game/binding/LuaGame.h
)

source_group("game" FILES ${MILK_SRC_GAME})

set(MILK_SRC_GRAPHICS
    src/graphics/Animator.cpp
    src/graphics/Animator.h
    src/graphics/Color.h
    src/graphics/Graphics.cpp
    src/graphics/Graphics.h
    src/graphics/Sprite.cpp
    src/graphics/Sprite.h
    src/graphics/Texture.cpp
    src/graphics/Texture.h
    src/graphics/Tilemap.h
)

source_group("graphics" FILES ${MILK_SRC_GRAPHICS})

set(MILK_SRC_MATH
    src/math/Mathf.h
    src/math/Matrix3.h
    src/math/Rectangle.h
    src/math/Vector2.h
    src/math/binding/LuaMathf.cpp
    src/math/binding/LuaMathf.h
    src/math/binding/LuaVector2.cpp
    src/math/binding/LuaVector2.h
)

source_group("math" FILES ${MILK_SRC_MATH})

set(MILK_SRC_PHYSICS
    src/physics/BoxCollider.cpp
    src/physics/BoxCollider.h
    src/physics/Collision.h
    src/physics/Physics.cpp
    src/physics/Physics.h
    src/physics/SpatialPartitionGrid.cpp
    src/physics/SpatialPartitionGrid.h
    src/physics/Velocity.cpp
    src/physics/Velocity.h
    src/physics/binding/LuaCollision.cpp
    src/physics/binding/LuaCollision.h
)

source_group("physics" FILES ${MILK_SRC_PHYSICS})

set(MILK_SRC_SCENE
    src/scene/Actor.cpp
    src/scene/Actor.h
    src/scene/ActorComponent.cpp
    src/scene/ActorComponent.h
    src/scene/Camera.cpp
    src/scene/Camera.h
    src/scene/Scene.cpp
    src/scene/Scene.h
    src/scene/binding/LuaActor.cpp
    src/scene/binding/LuaActor.h
    src/scene/binding/LuaScene.cpp
    src/scene/binding/LuaScene.h
)

source_group("scene" FILES ${MILK_SRC_SCENE})

set(MILK_SRC_SCRIPTING
    src/scripting/LuaApi.cpp
    src/scripting/LuaApi.h
    src/scripting/Logic.cpp
    src/scripting/Logic.h
    src/scripting/Script.cpp
    src/scripting/Script.h
)

source_group("scripting" FILES ${MILK_SRC_SCRIPTING})

set(MILK_SRC_UTILITIES
    src/utilities/Alignment.h
    src/utilities/IdGenerator.cpp
    src/utilities/IdGenerator.h
    src/utilities/Timer.cpp
    src/utilities/Timer.h
)

source_group("utilities" FILES ${MILK_SRC_UTILITIES})

set(MILK_SRC_WINDOW
    src/window/Renderer.h
    src/window/Window.h
    src/window/adapter/RendererAdapter.cpp
    src/window/adapter/RendererAdapter.h
    src/window/adapter/WindowAdapter.cpp
    src/window/adapter/WindowAdapter.h
    src/window/binding/LuaWindow.cpp
    src/window/binding/LuaWindow.h
)

source_group("window" FILES ${MILK_SRC_WINDOW})

set(MILK_SRC_FILES
    ${MILK_SRC_LIBS}
    ${MILK_SRC_ASSET}
    ${MILK_SRC_DEBUG_TOOLS}
    ${MILK_SRC_FILESYSTEM}
    ${MILK_SRC_INPUT}
    ${MILK_SRC_GAME}
    ${MILK_SRC_GRAPHICS}
    ${MILK_SRC_MATH}
    ${MILK_SRC_PHYSICS}
    ${MILK_SRC_SCENE}
    ${MILK_SRC_SCRIPTING}
    ${MILK_SRC_UTILITIES}
    ${MILK_SRC_WINDOW}
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

add_executable(milk main/main.cpp)

target_link_libraries(milk milklib)

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${milk_BINARY_DIR})

#---------------------------------------------------------------------------
# set start up project for visual studio
#---------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT milk)