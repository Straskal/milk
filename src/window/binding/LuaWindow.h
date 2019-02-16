#ifndef MILK_LUAWINDOW_H
#define MILK_LUAWINDOW_H

namespace sol
{
    class state;
}

namespace milk
{
    class Window;

    namespace lua
    {
        class LuaWindow
        {
        public:
            static void bind(sol::state& luaState);

            static int width(Window& window);

            static int height(Window& window);

            static bool fullscreen(Window& window);

            static void toggleFullscreen(Window& window);
        };
    }
}

#endif
