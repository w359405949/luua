#include <lauxlib.h>
#include <uv.h>
#include "luv_misc.h"
#include "utils.h"

int luv_loop_destroy(lua_State* L)
{
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, 1, UV_LOOP_METATABLE_NAME);
    uv_stop(loop);
    return 0;
}

int luv_loop_new(lua_State* L)
{
    uv_loop_t *loop = (uv_loop_t*)lua_newuserdata(L, sizeof(uv_loop_t));
    uv_loop_init(loop);

    /* metatable __gc */
    luua_setgcmetamethod(L, UV_LOOP_METATABLE_NAME, luv_loop_destroy);
    return 1;
}

int luv_run(lua_State* L)
{
    lua_settop(L, 3);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, 1, UV_LOOP_METATABLE_NAME);
    int flags = UV_RUN_DEFAULT;
    flags |= lua_toboolean(L, 2);
    flags |= lua_toboolean(L, 3);

    uv_run(loop, flags);
    return 0;
}

int luv_loop_stop(lua_State* L)
{
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, 1, UV_LOOP_METATABLE_NAME);
    uv_stop(loop);
    return 0;
}
