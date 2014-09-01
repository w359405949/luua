#include <lauxlib.h>
#include <uv.h>
#include "luv_misc.h"
#include "luv_prepare.h"
#include "utils.h"

int luv_prepare_destroy(lua_State* L)
{
    lua_settop(L, 1);
    luv_prepare_stop(L);
    return 0;
}

int luv_prepare_new(lua_State* L)
{
    lua_settop(L, 1);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, -1, UV_LOOP_METATABLE_NAME);
    uv_prepare_t* handle = (uv_prepare_t*)lua_newuserdata(L, sizeof(uv_prepare_t));
    uv_prepare_init(loop, handle);

    /* metatable __gc */
    luua_setgcmetamethod(L, UV_PREPARE_METATABLE_NAME, luv_prepare_destroy);
    return 1;
}

void prepare_cb(uv_prepare_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    lua_State* parent = (lua_State*)lua_touserdata(coroutine, 1);
    lua_pop(coroutine, 1);
    int result = lua_resume(coroutine, parent, 1);
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
    uv_prepare_stop(handle);
}

int luv_prepare_start(lua_State* L)
{
    lua_settop(L, 2);
    uv_prepare_t* handle = (uv_prepare_t*)luaL_checkudata(L, 1, UV_PREPARE_METATABLE_NAME);
    lua_getfield(L, 2, "coroutine");
    lua_getfield(L, 2, "parent");
    lua_State* coroutine = lua_tothread(L, 3);
    lua_State* parent = lua_tothread(L, 4);

    lua_settop(L, 2);
    lua_xmove(L, coroutine, 1);

    lua_pushlightuserdata(coroutine, parent);
    handle->data = coroutine;
    uv_prepare_start(handle, prepare_cb);
    return 0;
}

int luv_prepare_stop(lua_State* L)
{
    lua_settop(L, 1);
    uv_prepare_t* handle = (uv_prepare_t*)luaL_checkudata(L, 1, UV_PREPARE_METATABLE_NAME);
    uv_prepare_stop(handle);
    return 0;
}
