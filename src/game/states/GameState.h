#ifndef MILK_GAMESTATE_H
#define MILK_GAMESTATE_H

#include <memory>

namespace milk
{
    class Game;
    class GameEvent;

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
        virtual void begin() {};

        /// Called once at the beginning of each frame.
        /// \param gameEvent
        virtual void handleEvent(GameEvent& gameEvent) {};

        /// Called once per frame.
        /// \returns the new GameState or nullptr if there is not change
        virtual std::unique_ptr<GameState> checkState() = 0;

        /// Called once per frame.
        virtual void update() {};

        /// Called once at the end of each frame.
        virtual void render() {}

        /// Called once this state has ended
        virtual void end() {};

        /// \returns true if the state below on the stack will be rendered "behind" this state.
        virtual bool transparent() = 0;

    protected:
        Game& game_;
    };
}

#endif
