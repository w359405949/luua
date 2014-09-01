#include <stdlib.h>
#include <lauxlib.h>
#include <uv.h>
#include "luv_misc.h"
#include "luv_timer.h"
#include "utils.h"


int luv_timer_destroy(lua_State* L)
{
    lua_settop(L, 1);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    uv_timer_stop(handle);
    return 0;
}

int luv_timer_new(lua_State* L)
{
    lua_settop(L, 1);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, 1, UV_LOOP_METATABLE_NAME);
    uv_timer_t* handle = (uv_timer_t*)lua_newuserdata(L, sizeof(uv_timer_t));
    uv_timer_init(loop, handle);

    /* metatable __gc */
    luua_setgcmetamethod(L, UV_TIMER_METATABLE_NAME, luv_timer_destroy);
    return 1;
}


void timer_cb(uv_timer_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    lua_State* parent = luua_getparentcoroutine(coroutine, -1);
    int result = lua_resume(coroutine, parent, 1);
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
}

int luv_timer_start(lua_State* L)
{
    lua_settop(L, 4);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    lua_getfield(L, 2, "coroutine");
    uint64_t timeout = lua_tointeger(L, 3);
    uint64_t repeat = lua_tointeger(L, 4);
    lua_State* coroutine = lua_tothread(L, 5);

    //lua_settop(L, 2);
    //lua_xmove(L, coroutine, 1);

    handle->data = coroutine;
    uv_timer_start(handle, timer_cb, timeout, repeat);
    return 0;
}

int luv_timer_stop(lua_State* L)
{
    lua_settop(L, 1);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    uv_timer_stop(handle);
    return 0;
}

int luv_timer_again(lua_State* L)
{
    lua_settop(L, 1);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    uv_timer_again(handle);
    return 0;
}

int luv_timer_set_repeat(lua_State* L)
{
    lua_settop(L, 1);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    uint64_t repeat = lua_tointeger(L, 2);
    uv_timer_set_repeat(handle, repeat);
    return 0;
}

int luv_timer_get_repeat(lua_State* L)
{
    lua_settop(L, 1);
    uv_timer_t* handle = (uv_timer_t*)luaL_checkudata(L, 1, UV_TIMER_METATABLE_NAME);
    uint64_t repeat = uv_timer_get_repeat(handle);
    lua_pushinteger(L, repeat);
    return 1;
}
