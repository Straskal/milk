#ifndef MILK_LUA_API
#define MILK_LUA_API

struct lua_State;

namespace milk {
	struct MilkState;

	class LuaApi {
	public:
		static void init(lua_State* L, MilkState* milkState);

	private:
		static MilkState* milkState_;
	};
}

#endif