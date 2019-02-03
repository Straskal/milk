#ifndef MILK_GAME_H
#define MILK_GAME_H

// Exit codes
#define MILK_SUCCESS 0
#define MILK_FAIL 1

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "externals/sol.hpp"

namespace milk
{
    template<class T>
    class AssetCache;

#ifdef _DEBUG
    class DebugTools;
#endif

    class Filesystem;
    class GameState;
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

        /// \returns the game window.
        Window& window() const;

        /// \returns the games filesystem.
        Filesystem& filesystem() const;

        /// \returns the games resource manager.
        AssetCache<Texture>& textureCache() const;

        /// \returns the games lua state.
        sol::state& luaState();

#if _DEBUG
        /// \returns the Game's debug tools if in debug mode.
        DebugTools& debugTools() const;
#endif

        /// Attempts to load the requested Scene.
        /// \param name
        void loadScene(const std::string& name);

        /// Quits and shuts down the Game.
        void quit();

    private:
        std::stack<std::unique_ptr<GameState>> stateStack_;

        std::string configFile_;

        std::unique_ptr<adapter::WindowAdapter> window_;
        std::unique_ptr<adapter::FilesystemAdapter> fileSystem_;
        std::unique_ptr<adapter::TextureCacheAdapter> textureCache_;

        sol::state luaState_;

#ifdef _DEBUG
        std::unique_ptr<DebugTools> debugTools_;
#endif

        bool isRunning_;

        bool initFromConfig();
        void handleEvents();
        void update();
        void render();
        void shutDown();
    };
}

#endif
