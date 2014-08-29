#include <lauxlib.h>
#include <uv.h>
#include "luv_check.h"
#include "utils.h"

#define UV_CHECK_METATABLE_NAME "uv.uv_check_t"

int luv_check_destroy(lua_State* L)
{
    uv_check_t* handle = (uv_check_t*)luaL_checkudata(L, 1, UV_CHECK_METATABLE_NAME);
    uv_check_stop(handle);
    return 0;
}

int luv_check_new(lua_State* L)
{
    uv_loop_t* loop = (uv_loop_t*)lua_touserdata(L, -1);
    uv_check_t* handle = (uv_check_t*)lua_newuserdata(L, sizeof(uv_check_t));
    uv_check_init(loop, handle);
    lua_pushlightuserdata(L, handle);

    /* metatable __gc */
    luua_setgcmetamethod(L, UV_CHECK_METATABLE_NAME, luv_check_destroy);
    return 1;
}

void check_cb(uv_check_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    lua_State* parent = (lua_State*)lua_touserdata(coroutine, -1);
    lua_pop(coroutine, 1);
    int result = lua_resume(coroutine, parent, 1);
    if (result > 1) {
        printf("result:%d\n", result);
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
    //luaL_traceback(coroutine, coroutine, lua_tostring(coroutine, 1), 1);
}

int luv_check_start(lua_State* L)
{
    uv_check_t* handle = (uv_check_t*)luaL_checkudata(L, 1, UV_CHECK_METATABLE_NAME);
    lua_getfield(L, 2, "coroutine");
    lua_getfield(L, 2, "parent");
    lua_State* coroutine = lua_tothread(L, 3);
    lua_State* parent = lua_tothread(L, 4);

    lua_settop(L, 2);
    lua_xmove(L, coroutine, 1);

    lua_pushlightuserdata(coroutine, parent);
    handle->data = coroutine;
    uv_check_start(handle, check_cb);
    return 0;
}

int luv_check_stop(lua_State* L)
{
    uv_check_t* handle = (uv_check_t*)luaL_checkudata(L, 1, UV_CHECK_METATABLE_NAME);
    uv_check_stop(handle);
    return 0;
}
