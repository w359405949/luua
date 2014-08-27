#include <uv.h>
#include "luv_timer.h"
#include "utils.h"

int luv_timer_new(lua_State* L)
{
    uv_loop_t* loop = (uv_loop_t*)lua_touserdata(L, -1);
    uv_timer_t* handle = (uv_timer_t*)lua_newuserdata(L, sizeof(uv_timer_t));
    uv_timer_init(loop, handle);
    lua_pushlightuserdata(L, handle);
    return 1;
}

//TODO: check return and do somthing
void timer_cb(uv_timer_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    //lua_State* parent = (lua_State*)lua_touserdata(coroutine, 1);
    //lua_pop(coroutine, 1);
    //lua_resume(coroutine, parent, 0);
    lua_resume(coroutine, NULL, 0);
}

//TODO: more arguments support
int luv_timer_start(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_getfield(L, 2, "coroutine");
    lua_getfield(L, 2, "parent");
    uint64_t timeout = lua_tointeger(L, 3);
    uint64_t repeat = lua_tointeger(L, 4);
    lua_State* coroutine = lua_tothread(L, 5);
    //lua_State* parent = lua_tothread(L, 6);

    //lua_pushlightuserdata(coroutine, parent);
    handle->data = coroutine;
    uv_timer_start(handle, timer_cb, timeout, repeat);
    return 0;
}

int luv_timer_stop(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    uv_timer_stop(handle);
    return 0;
}

int luv_timer_again(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_newuserdata(L, 1);
    uv_timer_again(handle);
    return 0;
}

int luv_timer_set_repeat(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    uint64_t repeat = lua_tointeger(L, 2);
    uv_timer_set_repeat(handle, repeat);
    return 0;
}

int luv_timer_get_repeat(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    uint64_t repeat = uv_timer_get_repeat(handle);
    lua_pushinteger(L, repeat);
    return 1;
}
