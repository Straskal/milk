#ifndef MILK_GAMESTATE_SCENETRANSITION_H
#define MILK_GAMESTATE_SCENETRANSITION_H

#define NULL_SCENE "ABSOLUTELY_NO_GD_SCENE_BBY"

#include "GameState.h"

#include "scene/SceneLoader.h"

namespace milk
{
    class GameState_SceneTransition : public GameState
    {
    public:
        /// This state is active when one scene is transitioning to another
        explicit GameState_SceneTransition(Game& game);

        void begin() override;

        void handleEvent(GameEvent& gameEvent) override;

        std::unique_ptr<GameState> update() override;

        void render() override;

        void end() override;

        bool transparent() override;

    private:
        SceneLoader sceneLoader_;
    };
}

#endif
