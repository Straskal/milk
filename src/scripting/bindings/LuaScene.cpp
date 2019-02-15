#include "LuaScene.h"

#include "sol/sol.hpp"

#include "scene/Actor.h"
#include "scene/Scene.h"

namespace milk
{
    namespace lua
    {
        void LuaScene::bind(sol::state& luaState)
        {
            luaState.new_usertype<Scene>("Scene",
                                         "set_cam_pos", &LuaScene::setCameraPosition,
                                         "spawn", &LuaScene::spawn,
                                         "destroy", &LuaScene::destroy);
        }

        void LuaScene::setCameraPosition(Scene& scene, Vector2& position)
        {
            scene.camera().position(position.x, position.y);
        }

        Actor* LuaScene::spawn(Scene& scene, const std::string& name, const Vector2& position, const std::string& actorTemplateName)
        {
            return scene.spawnActor(name, position, actorTemplateName);
        }

        bool LuaScene::destroy(Scene& scene, Actor& actor)
        {
            return scene.destroyActor(actor.id());
        }
    }
}