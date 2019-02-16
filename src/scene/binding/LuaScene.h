#ifndef LUASCENE_H
#define LUASCENE_H

#include <string>

namespace sol
{
    class state;
}

namespace milk
{
    class Actor;
    class Scene;

    struct Vector2;

    namespace lua
    {
        class LuaScene
        {
        public:
            static void bind(sol::state& luaState);

            static void setCameraPosition(Scene& scene, Vector2& position);

            static Actor* spawn(Scene& scene, const std::string& name, const Vector2& position, const std::string& actorTemplateName);

            static bool destroy(Scene& scene, Actor& actor);
        };
    }
}

#endif