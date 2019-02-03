#ifndef LUASCENE_H
#define LUASCENE_H

namespace milk
{
    class Actor;
    class Scene;
    class Vector2d;

    namespace lua
    {
        namespace scene
        {
            void setCameraPosition(Scene& scene, Vector2d& position);
        }
    }
}

#endif