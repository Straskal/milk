project(milklib)

set(MILK_SRC_FILES

        # third party libs
        ../libs/json/json.hpp
        ../libs/sol/sol.hpp

        # asset cache
        assetcache/AssetCache.h
        assetcache/adapter/ActorTemplateCacheAdapter.cpp
        assetcache/adapter/ActorTemplateCacheAdapter.h
        assetcache/adapter/TextureCacheAdapter.cpp
        assetcache/adapter/TextureCacheAdapter.h

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

        # game
        game/Game.h
        game/Game.cpp

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
        math/Rectangle.h
        math/Vector2d.h

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

        #scene
        scene/Actor.cpp
        scene/Actor.h
        scene/ActorComponent.cpp
        scene/ActorComponent.h
        scene/ActorLoader.h
        scene/Camera.cpp
        scene/Camera.h
        scene/Scene.cpp
        scene/Scene.h
        scene/SceneLoader.h
        scene/adapter/ActorLoaderAdapter.cpp
        scene/adapter/ActorLoaderAdapter.h
        scene/adapter/ActorJsonDeserializer.h
        scene/adapter/SceneLoaderAdapter.cpp
        scene/adapter/SceneLoaderAdapter.h
        scene/adapter/SceneJsonDeserializer.h
        scene/adapter/deserializers-v1/ActorJsonDeserializerV1.cpp
        scene/adapter/deserializers-v1/ActorJsonDeserializerV1.h
        scene/adapter/deserializers-v1/SceneJsonDeserializerV1.cpp
        scene/adapter/deserializers-v1/SceneJsonDeserializerV1.h

        # scripting
        scripting/api/LuaApi.cpp
        scripting/api/LuaApi.h
        scripting/api/LuaActor.cpp
        scripting/api/LuaActor.h
        scripting/api/LuaCollision.cpp
        scripting/api/LuaCollision.h
        scripting/api/LuaGame.cpp
        scripting/api/LuaGame.h
        scripting/api/LuaScene.cpp
        scripting/api/LuaScene.h
        scripting/api/LuaWindow.cpp
        scripting/api/LuaWindow.h
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
        window/adapter/WindowAdapter.h)

add_library(milklib ${MILK_SRC_FILES})

# include lib headers
target_include_directories(milklib
        PUBLIC
            ${SDL_INCLUDE_DIR}
            ${SDL2_IMAGE_INCLUDE_DIRS}
            ${LUA53_INCLUDE_DIR}
            ${milklib_SOURCE_DIR}
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