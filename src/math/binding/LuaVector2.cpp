#include "LuaVector2.h"

#include "sol/sol.hpp"

#include "math/Vector2.h"

void milk::lua::LuaVector2::bind(sol::state& luaState)
{
    luaState.new_usertype<Vector2>("Vector2",
                                   sol::constructors<Vector2(), Vector2(int, int)>(),
                                   "x", &Vector2::x,
                                   "y", &Vector2::y,
                                   "magnitude", &Vector2::magnitude,
                                   "normalize", &Vector2::normalize,
                                   sol::meta_function::addition, sol::resolve<Vector2(const Vector2&, const Vector2&)>(milk::operator+),
                                   sol::meta_function::multiplication, sol::resolve<Vector2(const Vector2&, float)>(milk::operator*));
}
