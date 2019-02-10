#include "LuaApi.h"

#include "sol/sol.hpp"

#include "LuaActor.h"
#include "LuaCollision.h"
#include "LuaGame.h"
#include "LuaScene.h"
#include "LuaWindow.h"

#include "input/Keyboard.h"
#include "game/Game.h"
#include "math/Mathf.h"
#include "math/Vector2.h"
#include "physics/BoxCollider.h"
#include "physics/Collision.h"
#include "scene/Actor.h"
#include "scene/Scene.h"
#include "window/Window.h"

void milk::LuaApi::init(sol::state& luaState)
{
    // Input
    /////////////////////////////////////////////////////////////////
    luaState.new_enum("Keys",
                      "A", SDLK_a,
                      "D", SDLK_d,
                      "F", SDLK_f,
                      "R", SDLK_r,
                      "S", SDLK_s,
                      "W", SDLK_w,
                      "Space", SDLK_SPACE,
                      "Tilde", SDLK_BACKQUOTE);

    // Actor
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Actor>("Actor",
                                 "name", sol::readonly_property(&lua::actor::name),
                                 "position", sol::readonly_property(&lua::actor::position),
                                 "move", &lua::actor::move,
                                 "set_animation", &lua::actor::setAnimation,
                                 "flip_x", &lua::actor::flipX,
                                 "flip_y", &lua::actor::flipY);

    // Collision Event
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Collision>("Collision",
                                     "other", sol::readonly_property(&lua::collision_event::other));

    // Math Functions
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Mathf>("Mathf",
                                 "clamp", &Mathf::clamp);

    // Game
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Game>("Game",
                                "load_scene", &lua::game::loadScene);

    // Input
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Keyboard>("Input",
                                    "get_key", &Keyboard::getKey,
                                    "get_key_pressed", &Keyboard::getKeyPressed,
                                    "get_key_released", &Keyboard::getKeyReleased);

    // Scene
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Scene>("Scene",
                                 "set_cam_pos", &lua::scene::setCameraPosition,
                                 "spawn", &lua::scene::spawn);

    // Vector2D
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Vector2>("Vector2",
                                    sol::constructors<Vector2(), Vector2(int, int)>(),
                                    "x", &Vector2::x,
                                    "y", &Vector2::y,
                                    "magnitude", &Vector2::magnitude,
                                    "normalize", &Vector2::normalize,
                                    sol::meta_function::equal_to, &Vector2::operator==,
                                    sol::meta_function::multiplication, &Vector2::operator*);

    // Window
    /////////////////////////////////////////////////////////////////
    luaState.new_usertype<Window>("Window",
                                  "toggle_fullscreen", &lua::window::toggleFullscreen);
}