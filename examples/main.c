#include <lua.hpp>

int main()
{
    lua_State *L = luaL_newstate();
    lua_State *thread = lua_newthread(L);
    luaL_openlibs(L);
    lua_pushlightuserdata(thread, L);
    lua_setglobal(thread, "_parent");

    luaL_loadfile(thread, "test.lua");
    lua_resume(thread, NULL, 0);
    //printf("%s\n", lua_tostring(L, -1));
    lua_getglobal(thread, "_parent");
    lua_State *parent = (lua_State*)lua_touserdata(thread, -1);
    printf("%lld %lld %d\n", (long long)parent, (long long)L, parent == L);
    lua_close(L);
}
