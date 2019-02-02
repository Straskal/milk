#include "LuaScene.h"

#include "scene/Scene.h"

namespace milk
{
    namespace lua
    {
        namespace scene
        {
            void setCameraPosition(Scene& scene, Vector2d& position)
            {
                scene.camera().position(position.x, position.y);
            }
        }
    }
}