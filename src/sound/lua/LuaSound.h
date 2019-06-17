#ifndef _LUA_SOUND_H_
#define _LUA_SOUND_H_

struct lua_State;

namespace milk {
	struct Sound;

	class LuaSound {
	public:
		static const char* METATABLE;
		static void pushSoundTable(lua_State* L);
	};
}

#endif