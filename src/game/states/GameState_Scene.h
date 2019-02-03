#ifndef MILK_GAME_STATE_SCENE_H
#define MILK_GAME_STATE_SCENE_H

#include <string>

#include "GameState.h"

namespace milk
{
    class Scene;

    class GameState_Scene : public GameState
    {
    public:
        /// This state is active when a Scene is being processed
        explicit GameState_Scene(Game& game, Scene& scene);

        void begin() override;

        void handleEvent(GameEvent& gameEvent) override;

        std::unique_ptr<GameState> update() override;

        void render() override;

        void end() override;

        bool transparent() override;

    private:
        Scene& scene_;
    };
}

#endif
