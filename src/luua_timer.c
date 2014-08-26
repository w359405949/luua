

int luua_timer(lua_State* L)
{
    uv_loop *loop;
    uv_timer_t* handle = (uv_timer_t)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, handle);
    lua_pushlightuserdata(L, handle);
    return 1;
}

void timer_cb(uv_timer_t* handle)
{
    lua_State* L;
    lua_State* coroutine = (lua_State*)handle->data;
    lua_resume(coroutine, L, 0);
}

int luua_timer_start(lua_State* L)
{
    lua_State* coutine;
    uv_timer_t* handle;
    uint64_t timeout;
    uint64_t repeat;
    handle->data = coroutine;
    //TODO: keep arguments
    uv_timer_start(handle, timer_cb, timeout, repeat);
    return 0;
}

int luua_timer_stop(lua_State* L)
{
}

int luua_timer_again(lua_State* L)
{
}

int luua_timer_set_repeat(lua_State* L)
{
}

int luua_timer_get_repeat(lua_State* L)
{
}

int luua_timer_get_active(lua_State* L)
{
}
