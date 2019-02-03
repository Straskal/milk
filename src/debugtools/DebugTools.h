#ifndef MILK_DEBUG_TOOLS_H
#define MILK_DEBUG_TOOLS_H

#include <unordered_map>

namespace milk
{
    class Actor;
    class BoxCollider;
    class Renderer;
    class Scene;

    class DebugTools
    {
    public:
        /// As of right now, the debug tools only show the collision grid and colliders.
        /// In the future, this will contain a command line, level editor, animation editor, etc...
        /// \param renderer
        explicit DebugTools(Renderer& renderer);

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
