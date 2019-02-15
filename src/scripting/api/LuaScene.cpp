#include "LuaScene.h"

#include "scene/Actor.h"
#include "scene/Scene.h"

namespace milk
{
    namespace lua
    {
        namespace scene
        {
            void setCameraPosition(Scene& scene, Vector2& position)
            {
                scene.camera().position(position.x, position.y);
            }

            Actor* spawn(Scene& scene, const std::string& name, const Vector2& position, const std::string& actorTemplateName)
            {
                return scene.spawnActor(name, position, actorTemplateName);
            }

            bool destroy(Scene& scene, Actor& actor)
            {
                return scene.destroyActor(actor.id());
            }
        }
    }
}