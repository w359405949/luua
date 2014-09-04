#include <assert.h>
#include <stdlib.h>
#include <lauxlib.h>
#include <uv.h>

#include "luv_misc.h"
#include "luv_socket.h"
#include "utils.h"

void close_cb(uv_handle_t* handle)
{
    lua_State* coroutine = (lua_State*)handle->data;
    if (NULL == coroutine) {
        return;
    }

    int result = lua_resume(coroutine, NULL, 0);

    if (result > LUA_YIELD) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
        assert(0 && "read cb");
    }

    lua_settop(coroutine, 0);
}

int luv_close(lua_State* L)
{
    lua_settop(L, 2);
    uv_handle_t* handle = (uv_handle_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    lua_getfield(L, 2, "_coroutine");

    lua_State* coroutine = lua_tothread(L, 3);
    assert(coroutine != NULL && "no coroutine found");

    handle->data = coroutine;
    uv_close(handle, close_cb);
    return 0;
}


int luv_stream_destroy(lua_State* L)
{
    lua_settop(L, 1);
    uv_handle_t* handle = (uv_handle_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    uv_close(handle, close_cb); // TODO(ISSUE): may crash
    return 0;
}

int luv_tcp_new(lua_State* L)
{
    lua_settop(L, 1);
    uv_loop_t* loop = (uv_loop_t*)luaL_checkudata(L, -1, UV_LOOP_METATABLE_NAME);
    uv_tcp_t* handle = (uv_tcp_t*)lua_newuserdata(L, sizeof(uv_tcp_t));
    uv_tcp_init(loop, handle);

    luua_setgcmetamethod(L, UV_STREAM_METATABLE_NAME, luv_stream_destroy);
    return 1;
}

int luv_tcp_bind(lua_State* L)
{
    lua_settop(L, 4);
    uv_tcp_t* handle = (uv_tcp_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
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
    lua_State* coroutine = (lua_State*)req->handle->data;
    int result = 0;
    if (status != 0) {
        lua_pushstring(coroutine, uv_strerror(status));
        result = lua_resume(coroutine, NULL, 1);
    } else {
        result = lua_resume(coroutine, NULL, 0);
    }
    if (result > LUA_YIELD) {
        luua_stackDump(coroutine);
        uv_stop(req->handle->loop);
        assert(0 && "connect cb");
    }

    free(req);
    lua_settop(coroutine, 0);
}

int luv_tcp_connect(lua_State* L)
{
    lua_settop(L, 4);
    uv_tcp_t* handle = (uv_tcp_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    lua_getfield(L, 2, "_coroutine");
    const char* host = luaL_checkstring(L, 3);
    int port = luaL_checkint(L, 4);
    lua_State* coroutine = lua_tothread(L, 5);
    assert(coroutine != NULL && "no coroutine found");

    struct sockaddr_in address;
    uv_ip4_addr(host, port, &address);
    uv_connect_t* req = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    handle->data = coroutine;

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
    uv_tcp_t* handle = (uv_tcp_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
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
    uv_tcp_t* handle = (uv_tcp_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
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
    lua_State* coroutine = (lua_State*)handle->data;
    int result = 0;
    if (status == 0) {
        uv_tcp_t* peer = (uv_tcp_t*)lua_newuserdata(coroutine, sizeof(uv_tcp_t));
        uv_tcp_init(handle->loop, peer);
        luua_setgcmetamethod(coroutine, UV_STREAM_METATABLE_NAME, luv_stream_destroy);

        result = uv_accept(handle, (uv_stream_t*)peer);
        if (result != 0) {
            return;
        }
        result = lua_resume(coroutine, NULL, 1);
    }
    if (result > LUA_YIELD) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
        assert(0 && "listen cb");
    }

    lua_settop(coroutine, 0);
}

int luv_listen(lua_State* L)
{
    lua_settop(L, 3);
    uv_stream_t* handle = (uv_stream_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    lua_getfield(L, 2, "_coroutine");
    int backlog = luaL_checkint(L, 3);
    lua_State* coroutine = lua_tothread(L, 4);

    assert(coroutine != NULL && "no coroutine found");

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
    uv_read_stop(handle);

    lua_State* coroutine = (lua_State*)handle->data;
    int result = 0;
    if (nread >= 0) {
        lua_pushlstring(coroutine, buf->base, nread);
        result = lua_resume(coroutine, NULL, 1);
    } else {
        lua_pushnil(coroutine);
        lua_pushfstring(coroutine, "read: %s", uv_strerror(nread));
        result = lua_resume(coroutine, NULL, 2);
    }

    if (result > LUA_YIELD) {
        luua_stackDump(coroutine);
        uv_stop(handle->loop);
        assert(0 && "read cb");
    }

    lua_settop(coroutine, 0);
    free(buf->base);
}

int luv_read_start(lua_State* L)
{
    lua_settop(L, 2);

    uv_stream_t* handle = (uv_stream_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    lua_getfield(L, 2, "_coroutine");
    lua_State* coroutine = lua_tothread(L, 3);

    assert(coroutine != NULL && "no coroutine found");

    handle->data = coroutine;
    uv_read_start(handle, alloc_cb, read_cb);
    return 0;
}

int luv_read_stop(lua_State* L)
{
    lua_settop(L, 1);
    uv_stream_t* handle = (uv_stream_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    uv_read_stop(handle);
    return 0;
}

void write_cb(uv_write_t* handle, int status)
{
    lua_State* coroutine = (lua_State*)handle->data;

    int result = 0;
    if (status) {
        lua_pushfstring(coroutine, "write: %s", uv_strerror(status));
        result = lua_resume(coroutine, NULL, 2);
    } else {
        result = lua_resume(coroutine, NULL, 0);
    }

    if (result > LUA_YIELD) {
        luua_stackDump(coroutine);
        assert(0 && "read cb");
    }

    free(handle);
    lua_settop(coroutine, 0);
}

int luv_write(lua_State* L)
{
    lua_settop(L, 3);
    uv_stream_t* handle = (uv_stream_t*)luaL_checkudata(L, 1, UV_STREAM_METATABLE_NAME);
    lua_getfield(L, 2, "_coroutine");
    size_t length;
    const char* data = luaL_checklstring(L, 3, &length);
    lua_State* coroutine = lua_tothread(L, 4);

    assert(coroutine != NULL && "no coroutine found");

    uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));

    req->data = coroutine;
    uv_buf_t buf;
    buf.base = (char*)data;
    buf.len = length;

    int result = uv_write(req, handle, &buf, 1, write_cb);

    if (result) {
        free(req);
        lua_pushfstring(L, "write: %s", uv_strerror(result));
        return 1;
    }

    return 0;
}

int luv_shutdown(lua_State* L)
{
    return 0;
}
