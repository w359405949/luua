#include <uv.h>
#include "luv_misc.h"

int luv_loop_new(lua_State* L)
{
    uv_loop_t *loop = uv_loop_new();
    lua_pushlightuserdata(L, loop);
    return 1;
}

int luv_run(lua_State* L)
{
    uv_loop_t *loop = lua_touserdata(L, 1);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
