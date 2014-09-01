#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include "utils.h"

void luua_setgcmetamethod(lua_State* L, const char* tname, lua_CFunction gc)
{
    luaL_newmetatable(L, tname);
    lua_pushcfunction(L, gc);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
}

lua_State* luua_getparentcoroutine(lua_State* L, int index)
{
    int top = lua_gettop(L);
    if (index > top || !lua_istable(L, index)) {
        return NULL;
    }

    lua_getfield(L, index, "parent");
    lua_getfield(L, -1, "coroutine");
    lua_State* coroutine = lua_tothread(L, -1);
    lua_settop(L, top);
    return coroutine;
}

void luua_stackDump(lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for (i=1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                {
                    printf("(%d, %d) %s\n", i, t, lua_tostring(L, i));
                    break;
                }
            case LUA_TBOOLEAN:
                {
                    printf("(%d, %d) %d\n", i, t, lua_toboolean(L, i));
                    break;
                }
            case LUA_TNUMBER:
                {
                    printf("(%d, %d) %g\n", i, t, lua_tonumber(L, i));
                    break;
                }
            case LUA_TTABLE:
                {
                    printf("(%d, %d) %s\n", i, t, lua_typename(L, t));
                    lua_pushnil(L);
                    while (lua_next(L, i) != 0) {
                        if (lua_type(L, -2) == LUA_TSTRING)
                        {
                            printf("    %s - %s - %s, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)), lua_tostring(L, -2));
                        }
                        else if (lua_type(L, -2) == LUA_TNUMBER)
                        {
                            printf("    %s - %s - %g, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)), lua_tonumber(L, -2));
                        }
                        else
                        {
                            printf("    %s - %s, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
                        }
                        lua_pop(L, 1);
                    }
                    break;
                }
            default:
                {
                    printf("(%d, %d) %s\n", i, t, lua_typename(L, t));
                    break;
                }
        }
    }
}
