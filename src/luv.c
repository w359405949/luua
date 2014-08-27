#include <lauxlib.h>
#include "luv.h"
#include "utils.h"

int luaopen_libluv(lua_State* L)
{
    lua_newtable(L);
    luaL_setfuncs(L, luv, 0);
    return 1;
}
