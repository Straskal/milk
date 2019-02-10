#ifndef LUASCENE_H
#define LUASCENE_H

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
        }
    }
}

#endif