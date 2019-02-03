#include <utility>

#ifndef MILK_GAMESTATE_H
#define MILK_GAMESTATE_H

#include <string>
#include <memory>

namespace milk
{
    class Game;

    class GameState
    {
    public:
        /// A State in which the Game can be in.
        /// \param game: THE Game
        explicit GameState(Game& game)
                : game_(game)
        {
        }

        /// Called once when the Game enters the State.
        virtual void begin()
        {
        };

        /// Called once per frame.
        /// \returns the new GameState or nullptr if there is not change
        virtual std::unique_ptr<GameState> checkState() = 0;

        /// Called once per frame.
        virtual void update()
        {
        };

        /// Called once at the end of each frame.
        virtual void render()
        {
        }

        /// Called once this state has ended
        virtual void end()
        {
        };

        /// \returns true if the state below on the stack will be rendered "behind" this state.
        virtual bool transparent() = 0;

        /// Sets the State's Scene to load. This doesn't necessarily mean that the State has to load this scene.
        /// For instance, the SceneTransition state. You cannot load a Scene FROM that state.
        /// Most other states do support this. It's a bit gross that this lives in GameState base class, but it works just fine for now.
        /// \param sceneToLoad
        virtual void loadScene(const std::string& sceneToLoad)
        {
        }

    protected:
        Game& game_;
    };
}

#endif
