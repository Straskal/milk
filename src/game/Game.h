#ifndef MILK_GAME_H
#define MILK_GAME_H

// Exit codes
#define MILK_SUCCESS 0
#define MILK_FAIL 1

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "externals/sol.hpp"

namespace milk
{
    template<class T>
    class AssetCache;

#ifdef _DEBUG
    class DebugTools;
#endif

    class EventQueue;
    class Filesystem;
    class GameState;
    class Graphics;
    class Logic;
    class Physics;
    class Scene;
    class SceneLoader;
    class Texture;
    class Window;

    namespace adapter
    {
        class FilesystemAdapter;
        class TextureCacheAdapter;
        class WindowAdapter;
    }

    class Game
    {
    public:
        // Game states
        friend class GameState_Scene;
        friend class GameState_SceneTransition;

        Game();

        /// The Game is in charge of startup, shutdown, state and scene management.
        /// \param configFile: Path to the config file
        explicit Game(const std::string& configFile);

        ~Game();

        /// Initializes and runs the game
        /// Returns MILK_SUCCESS on successful run
        /// Returns MILK_FAIL on unsuccessful run
        int run();

        /// Changes the Game's current state.
        /// \param state: The new state of the Game.
        void changeState(std::unique_ptr<GameState> state);

        /// Pushes a GameState onto the top of the GameState stack.
        /// \param state: The state to push onto the Game's state stack.
        void pushState(std::unique_ptr<GameState> state);

        /// Pops a GameState off of the game state stack.
        void popState();

        // Returns the game window.
        Window& window() const;

        // Returns the games filesystem.
        Filesystem& filesystem() const;

        // Returns the games resource manager.
        AssetCache<Texture>& textureCache() const;

        // Returns the games event queue.
        EventQueue& events() const;

        /// \returns the Game's current Scene.
        Scene* currentScene() const;

        /// Loads the requested Scene.
        /// \param sceneToLoad: The Scene to load.
        void loadScene(const std::string& sceneToLoad);

        /// Quits the Game and shuts down all sub systems.
        void quit();

    private:
        std::vector<std::unique_ptr<GameState>> states_;

        std::unique_ptr<Scene> currentScene_;
        std::string sceneToLoad_;

        std::string configFile_;

        std::unique_ptr<adapter::WindowAdapter> window_;
        std::unique_ptr<adapter::FilesystemAdapter> fileSystem_;
        std::unique_ptr<adapter::TextureCacheAdapter> textureCache_;

        std::unique_ptr<EventQueue> events_;

        sol::state luaState_;

#ifdef _DEBUG
        std::unique_ptr<DebugTools> debugTools_;
#endif

        std::unique_ptr<Logic> logic_;
        std::unique_ptr<Physics> physics_;
        std::unique_ptr<Graphics> graphics_;

        bool isRunning_;

        bool initFromConfig();
        void handleEvents();
        void update();
        void render();
        void shutDown();
    };
}

#endif
