#ifndef MILK_GAME_H
#define MILK_GAME_H

// Exit codes
#define MILK_SUCCESS 0
#define MILK_FAIL 1

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "sol/sol.hpp"
#include "json/json.hpp"

namespace milk
{
    template<class T>
    class AssetCache;

#ifdef _DEBUG
    class DebugTools;
#endif

    class Filesystem;
    class Graphics;
    class Logic;
    class Physics;
    class Renderer;
    class Scene;
    class Texture;
    class Window;

    namespace adapter
    {
        class ActorTemplateCacheAdapter;
        class FilesystemAdapter;
        class RendererAdapter;
        class SceneLoaderAdapter;
        class TextureCacheAdapter;
        class WindowAdapter;
    }

    /// The Game is in charge of startup, shutdown, state and scene management.
    class Game
    {
    public:
        /// \returns the single instance of the Game.
        static Game& getInstance()
        {
            static Game instance;
            return instance;
        }

        ~Game();

        /// Initializes the Game and its subsystems.
        /// \param configFilepath: The path to the Game's configuration file.
        void init(std::string configFilepath);

        /// Initializes and runs the game
        /// Returns MILK_SUCCESS on successful run
        /// Returns MILK_FAIL on unsuccessful run
        int run();

        /// \returns the game window.
        Window& window() const;

        /// \returns the games filesystem.
        Filesystem& filesystem() const;

        /// \returns the games texture cache.
        AssetCache<Texture>& textureCache() const;

        /// \returns the game actor template cache.
        AssetCache<nlohmann::json>& actorTemplateCache() const;

        /// \returns the games lua state.
        sol::state& luaState();

        /// Attempts to load the requested Scene.
        /// \param name
        void loadScene(const std::string& name);

        /// Quits and shuts down the Game.
        void quit();

    private:
        Game();

        std::string sceneToLoad_;
        std::unique_ptr<Scene> scene_;

        adapter::WindowAdapter* window_;
        adapter::RendererAdapter* renderer_;
        adapter::FilesystemAdapter* fileSystem_;
        adapter::SceneLoaderAdapter* sceneLoader_;
        adapter::TextureCacheAdapter* textureCache_;
        adapter::ActorTemplateCacheAdapter* actorTemplateCache_;

        sol::state luaState_;

        std::unique_ptr<Logic> logic_;
        std::unique_ptr<Physics> physics_;
        std::unique_ptr<Graphics> graphics_;

#ifdef _DEBUG
        std::unique_ptr<DebugTools> debugTools_;
#endif

        bool initialized_;
        bool running_;

        void handleEvents();
        void update();
        void render();
        void shutDown();
    };
}

#endif
