#include <lua.h>
#include <stdio.h>

void stackDump(lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for (i=1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                {
                    printf("(%d, %d) %s", i, t, lua_tostring(L, i));
                    break;
                }
            case LUA_TBOOLEAN:
                {
                    printf("(%d, %d) %d", i, t, lua_toboolean(L, i));
                    break;
                }
            case LUA_TNUMBER:
                {
                    printf("(%d, %d) %g", i, t, lua_tonumber(L, i));
                    break;
                }
            case LUA_TTABLE:
                {
                    lua_pushnil(L);
                    while (lua_next(L, i) != 0) {
                        if (lua_type(L, -2) == LUA_TSTRING)
                        {
                            printf("%s - %s - %s, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)), lua_tostring(L, -2));
                        }
                        else if (lua_type(L, -2) == LUA_TNUMBER)
                        {
                            printf("%s - %s - %g, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)), lua_tonumber(L, -2));
                        }
                        else
                        {
                            printf("%s - %s, \n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
                        }
                        lua_pop(L, 1);
                    }
                    break;
                }
            default:
                {
                    printf("(%d, %d) %s", i, t, lua_typename(L, t));
                    break;
                }
                printf(" ");
        }
        printf("\n");
    }
}
