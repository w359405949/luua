#include <lauxlib.h>
#include <uv.h>
#include "luv_misc.h"
#include "luv_check.h"
#include "utils.h"

int luv_check_destroy(lua_State* L)
{
    lua_settop(L, 1);
    luv_check_stop(L);
    return 0;
}

int luv_check_new(lua_State* L)
{
    lua_settop(L, 1);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, 1, UV_LOOP_METATABLE_NAME);
    uv_check_t* handle = (uv_check_t*)lua_newuserdata(L, sizeof(uv_check_t));
    uv_check_init(loop, handle);

    /* metatable __gc */
    luua_setgcmetamethod(L, UV_CHECK_METATABLE_NAME, luv_check_destroy);
    return 1;
}

void check_cb(uv_check_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    int result = lua_resume(coroutine, NULL, 1);
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
    uv_check_stop(handle);
}

int luv_check_start(lua_State* L)
{
    lua_settop(L, 2);
    uv_check_t* handle = (uv_check_t*)luaL_checkudata(L, 1, UV_CHECK_METATABLE_NAME);
    lua_getfield(L, 2, "coroutine");
    lua_State* coroutine = lua_tothread(L, 3);

    //lua_settop(L, 2);
    //lua_xmove(L, coroutine, 1);

    handle->data = coroutine;
    uv_check_start(handle, check_cb);
    return 0;
}

int luv_check_stop(lua_State* L)
{
    lua_settop(L, 1);
    uv_check_t* handle = (uv_check_t*)luaL_checkudata(L, 1, UV_CHECK_METATABLE_NAME);
    uv_check_stop(handle);
    return 0;
}
