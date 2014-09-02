#include <stdlib.h>
#include <lauxlib.h>
#include <uv.h>

#include "luv_misc.h"
#include "luv_socket.h"
#include "utils.h"

int luv_tcp_new(lua_State* L)
{
    lua_settop(L, 1);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, -1, UV_LOOP_METATABLE_NAME);
    uv_tcp_t* handle = (uv_tcp_t*)lua_newuserdata(L, sizeof(uv_tcp_t));

    uv_tcp_init(loop, handle);
    return 1;
}

int luv_tcp_bind(lua_State* L)
{
    lua_settop(L, 4);
    uv_tcp_t* handle = (uv_tcp_t*)lua_touserdata(L, 1);
    const char* host = luaL_checkstring(L, 2);
    int port = luaL_checkint(L, 3);
    int flags = lua_tointeger(L, 4);

    struct sockaddr_in address;
    uv_ip4_addr(host, port, &address);

    int result = uv_tcp_bind(handle, (struct sockaddr*)&address, flags);
    if (result) {
        lua_pushfstring(L, "bind: %s", uv_strerror(result));
        return 1;
    }
    return 0;
}

void connect_cb(uv_connect_t* req, int status)
{
    free(req);

    lua_State* coroutine = (lua_State*)req->handle->data;
    lua_pushinteger(coroutine, status);
    int result = lua_resume(coroutine, NULL, 1);
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(req->handle->loop);
    }
}

int luv_tcp_connect(lua_State* L)
{
    lua_settop(L, 3);
    uv_tcp_t* handle = (uv_tcp_t*)lua_touserdata(L, 1);
    const char* host = luaL_checkstring(L, 2);
    int port = luaL_checkint(L, 3);


    struct sockaddr_in address;
    uv_ip4_addr(host, port, &address);

    uv_connect_t* req = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    int result = uv_tcp_connect(req, handle, (struct sockaddr*)&address, connect_cb);

    if (result) {
        free(req);
        lua_pushfstring(L, "connect: %s", uv_strerror(result));
        return 1;
    }

    return 0;
}

int luv_tcp_nodelay(lua_State* L)
{
    lua_settop(L, 2);
    uv_tcp_t* handle = (uv_tcp_t*)lua_touserdata(L, 1);
    int enable = lua_toboolean(L, 2);

    int result = uv_tcp_nodelay(handle, enable);
    if (result) {
        lua_pushfstring(L, "nodelay: %s", uv_strerror(result));
        return 1;
    }
    return 0;
}

int luv_tcp_keepalive(lua_State* L)
{
    lua_settop(L, 3);
    uv_tcp_t* handle = (uv_tcp_t*)lua_touserdata(L, 1);
    int enable = lua_toboolean(L, 2);
    int delay = lua_toboolean(L, 3);

    int result = uv_tcp_keepalive(handle, enable, delay);

    if (result) {
         lua_pushfstring(L, "keepalive: %s", uv_strerror(result));
         return 1;
    }
    return 0;
}

int luv_tcp_getsockname(lua_State* L)
{
    return 0;
}

int luv_tcp_getpeername(lua_State* L)
{
    return 0;
}

void listen_cb(uv_stream_t* handle, int status)
{
    printf("new connection:%d\n", status);
    lua_State* coroutine = (lua_State*)handle->data;
    int result = 0;
    if (status == 0) {
        uv_tcp_t* peer = (uv_tcp_t*)lua_newuserdata(coroutine, sizeof(uv_tcp_t));
        uv_tcp_init(handle->loop, peer);
        result = uv_accept(handle, (uv_stream_t*)peer);
        if (result != 0) {
            return;
        }
        result = lua_resume(coroutine, NULL, 1);
    }
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
}

int luv_listen(lua_State* L)
{
    lua_settop(L, 3);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 1);
    lua_getfield(L, 2, "coroutine");
    int backlog = luaL_checkint(L, 3);
    lua_State* coroutine = lua_tothread(L, 4);

    handle->data = coroutine;
    int result = uv_listen(handle, backlog, listen_cb);
    if (result) {
        lua_pushfstring(L, "listen: %s", uv_strerror(result));
        return 1;
    }
    return 0;
}

void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
    lua_State* coroutine = (lua_State*)handle->data;
    if (nread >= 0) {
        lua_pushlstring(coroutine, buf->base, nread);
    }
    free(buf->base);

    int result = lua_resume(coroutine, NULL, 1);
    if (result > 1) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
    }
}

int luv_read_start(lua_State* L)
{
    lua_settop(L, 2);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 1);
    handle->data = NULL;
    uv_read_start(handle, alloc_cb, read_cb);
    return 0;
}

int luv_read_stop(lua_State* L)
{
    lua_settop(L, 1);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 1);
    uv_read_stop(handle);
    return 0;
}

int luv_write(lua_State* L)
{
    return 0;
}

int luv_close(lua_State* L)
{
    return 0;
}

int luv_shutdown(lua_State* L)
{
    return 0;
}
