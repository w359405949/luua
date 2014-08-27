#include <uv.h>
#include "luv_check.h"

int luv_check_new(lua_State* L)
{
    uv_loop_t* loop = (uv_loop_t*)lua_touserdata(L, -1);
    uv_check_t* handle = (uv_check_t*)lua_newuserdata(L, sizeof(uv_check_t));
    uv_check_init(loop, handle);

    lua_pushlightuserdata(L, handle);
    return 1;
}

void check_cb(uv_check_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    lua_State* parent = (lua_State*)lua_touserdata(coroutine, 1);
    lua_pop(coroutine, 1);
    lua_resume(coroutine, parent, 0);
}

int luv_check_start(lua_State* L)
{
    uv_check_t* handle = (uv_check_t*)lua_touserdata(L, 1);
    lua_getfield(L, 2, "coroutine");
    lua_getfield(L, 2, "parent");
    lua_State* coroutine = lua_tothread(L, 5);
    lua_State* parent = lua_tothread(L, 6);

    lua_pushlightuserdata(coroutine, parent);
    handle->data = coroutine;
    uv_check_start(handle, check_cb);
    return 0;
}

int luv_check_stop(lua_State* L)
{
    uv_check_t* handle = (uv_check_t*)lua_touserdata(L, 1);
    uv_check_stop(handle);
    return 0;
}
