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
        math/Matrix3.h
        math/Rectangle.h
        math/Vector2.h

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
        scene/Camera.cpp
        scene/Camera.h
        scene/Scene.cpp
        scene/Scene.h

        # scripting
        scripting/LuaApi.cpp
        scripting/LuaApi.h
        scripting/Logic.cpp
        scripting/Logic.h
        scripting/Script.cpp
        scripting/Script.h
        scripting/bindings/LuaActor.cpp
        scripting/bindings/LuaActor.h
        scripting/bindings/LuaCollision.cpp
        scripting/bindings/LuaCollision.h
        scripting/bindings/LuaGame.cpp
        scripting/bindings/LuaGame.h
        scripting/bindings/LuaKeyboard.cpp
        scripting/bindings/LuaKeyboard.h
        scripting/bindings/LuaMathf.cpp
        scripting/bindings/LuaMathf.h
        scripting/bindings/LuaScene.cpp
        scripting/bindings/LuaScene.h
        scripting/bindings/LuaVector2.cpp
        scripting/bindings/LuaVector2.h
        scripting/bindings/LuaWindow.cpp
        scripting/bindings/LuaWindow.h

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