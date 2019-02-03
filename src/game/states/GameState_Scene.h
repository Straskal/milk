#ifndef MILK_GAME_STATE_SCENE_H
#define MILK_GAME_STATE_SCENE_H

#include <memory>
#include <string>

#include "GameState.h"

namespace milk
{
    class Logic;
    class Graphics;
    class Physics;
    class Scene;

    class GameState_Scene : public GameState
    {
    public:
        /// This state is active when a Scene is being processed
        explicit GameState_Scene(Game& game, std::unique_ptr<Scene> scene);

        ~GameState_Scene();

        void begin() override;

        std::unique_ptr<GameState> checkState() override;

        void update() override;

        void render() override;

        void end() override;

        bool transparent() override;

        void loadScene(const std::string& sceneToLoad) override;

    private:
        std::unique_ptr<Scene> scene_;

        std::string sceneToLoad_;

        std::unique_ptr<Logic> logic_;
        std::unique_ptr<Physics> physics_;
        std::unique_ptr<Graphics> graphics_;
    };
}

#endif
