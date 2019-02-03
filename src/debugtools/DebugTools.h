#ifndef MILK_DEBUG_TOOLS_H
#define MILK_DEBUG_TOOLS_H

#include <unordered_map>

namespace milk
{
    class Actor;
    class BoxCollider;
    class GameEvent;
    class Renderer;
    class Scene;

    class DebugTools
    {
    public:
        explicit DebugTools(Renderer& renderer);

        void handleEvent(GameEvent& gameEvent);

        void render(Scene& scene);

        void onActorSpawned(Actor& actor);

        void onActorDestroyed(Actor& actor);

        void flush();

        bool show = false;

    private:
        Renderer& renderer_;

        std::unordered_map<int, Actor*> actorsById_;

        void onSceneChanged();
    };
}

#endif
