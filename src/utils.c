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
                    printf("%s", lua_tostring(L, i));
                    break;
                }
            case LUA_TBOOLEAN:
                {
                    printf("%d", lua_toboolean(L, i));
                    break;
                }
            case LUA_TNUMBER:
                {
                    printf("%g", lua_tonumber(L, i));
                    break;
                }
            default:
                {
                    printf("%s", lua_typename(L, i));
                    break;
                }
                printf(" ");
        }
        printf("\n");
    }
}
