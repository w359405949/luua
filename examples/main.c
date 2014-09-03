#include <lua.h>
#include <lauxlib.h>

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfile(L, "test.lua");
    while (1) {
        lua_pushfstring(L, "%d", lua_gettop(L));
        lua_resume(L, NULL, 1);
        printf("back\n");
    }
    lua_close(L);
}
