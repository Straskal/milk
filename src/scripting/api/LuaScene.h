#ifndef LUASCENE_H
#define LUASCENE_H

#include <string>

namespace milk
{
    class Actor;
    class Scene;
    class Vector2;

    namespace lua
    {
        namespace scene
        {
            void setCameraPosition(Scene& scene, Vector2& position);

            Actor* spawn(Scene& scene, const std::string& name, const Vector2& position, const std::string& actorTemplateName);
        }
    }
}

#endif