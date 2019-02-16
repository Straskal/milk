project(milklib)

set(MILK_SRC_FILES

        # third party libs
        ../libs/json/json.hpp
        ../libs/sol/sol.hpp

        # asset cache
        asset/AssetCache.h
        asset/ActorLoader.h
        asset/SceneLoader.h
        asset/adapter/ActorLoaderAdapter.cpp
        asset/adapter/ActorLoaderAdapter.h
        asset/adapter/ActorJsonDeserializer.h
        asset/adapter/ActorTemplateCacheAdapter.cpp
        asset/adapter/ActorTemplateCacheAdapter.h
        asset/adapter/SceneLoaderAdapter.cpp
        asset/adapter/SceneLoaderAdapter.h
        asset/adapter/SceneJsonDeserializer.h
        asset/adapter/TextureCacheAdapter.cpp
        asset/adapter/TextureCacheAdapter.h
        asset/adapter/deserializers-v1/ActorJsonDeserializerV1.cpp
        asset/adapter/deserializers-v1/ActorJsonDeserializerV1.h
        asset/adapter/deserializers-v1/SceneJsonDeserializerV1.cpp
        asset/adapter/deserializers-v1/SceneJsonDeserializerV1.h

        # debug tools
        debugtools/DebugTools.cpp
        debugtools/DebugTools.h

        # filesystem
        filesystem/Filesystem.h
        filesystem/adapter/FilesystemAdapter.cpp
        filesystem/adapter/FilesystemAdapter.h

        # input
        input/Keyboard.cpp
        input/Keyboard.h
        input/binding/LuaKeyboard.cpp
        input/binding/LuaKeyboard.h

        # game
        game/Game.h
        game/Game.cpp
        game/binding/LuaGame.cpp
        game/binding/LuaGame.h

        # graphics
        graphics/Animator.cpp
        graphics/Animator.h
        graphics/Color.h
        graphics/Graphics.cpp
        graphics/Graphics.h
        graphics/Sprite.cpp
        graphics/Sprite.h
        graphics/Texture.cpp
        graphics/Texture.h
        graphics/Tilemap.h

        # math
        math/Mathf.h
        math/Matrix3.h
        math/Rectangle.h
        math/Vector2.h
        math/binding/LuaMathf.cpp
        math/binding/LuaMathf.h
        math/binding/LuaVector2.cpp
        math/binding/LuaVector2.h

        # physics
        physics/BoxCollider.cpp
        physics/BoxCollider.h
        physics/Collision.h
        physics/Physics.cpp
        physics/Physics.h
        physics/SpatialPartitionGrid.cpp
        physics/SpatialPartitionGrid.h
        physics/Velocity.cpp
        physics/Velocity.h
        physics/binding/LuaCollision.cpp
        physics/binding/LuaCollision.h

        #scene
        scene/Actor.cpp
        scene/Actor.h
        scene/ActorComponent.cpp
        scene/ActorComponent.h
        scene/Camera.cpp
        scene/Camera.h
        scene/Scene.cpp
        scene/Scene.h
        scene/binding/LuaActor.cpp
        scene/binding/LuaActor.h
        scene/binding/LuaScene.cpp
        scene/binding/LuaScene.h

        # scripting
        scripting/LuaApi.cpp
        scripting/LuaApi.h
        scripting/Logic.cpp
        scripting/Logic.h
        scripting/Script.cpp
        scripting/Script.h

        # utilities
        utilities/Alignment.h
        utilities/IdGenerator.cpp
        utilities/IdGenerator.h
        utilities/Timer.cpp
        utilities/Timer.h

        # window
        window/Renderer.h
        window/Window.h
        window/adapter/RendererAdapter.cpp
        window/adapter/RendererAdapter.h
        window/adapter/WindowAdapter.cpp
        window/adapter/WindowAdapter.h
        window/binding/LuaWindow.cpp
        window/binding/LuaWindow.h)

add_library(milklib ${MILK_SRC_FILES})

# include lib headers
target_include_directories(milklib
        PUBLIC
            ${SDL2_INCLUDE_DIR}
            ${SDL2_IMAGE_INCLUDE_DIRS}
            ${LUA53_INCLUDE_DIR}
            ${milklib_SOURCE_DIR}
            ../libs
        )

# visual studio needs both the .dll and .lib
target_link_directories(milklib
	PUBLIC
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${LUA53_INCLUDE_DIR}
        ${milklib_SOURCE_DIR}
        # not sure if we have to include ../libs here.
        ../libs
)

# link libs
target_link_libraries(milklib
        PUBLIC
            ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARIES}
            ${LUA53_LIBRARIES}
        PRIVATE
            # Include code-coverage settings
            coverage_config
        )