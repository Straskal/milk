#ifndef _LUA_TEXTURE_H_
#define _LUA_TEXTURE_H_

struct lua_State;

namespace milk {
	struct Texture;

	struct TextureHandle {
		Texture* texture;
	};

	class LuaTexture {
	public:
		static const char* METATABLE;

		static void pushTextureTable(lua_State* L);
	};
}

#endif