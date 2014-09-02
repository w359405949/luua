#ifndef LUV_SOCKET
#define LUV_SOCKET

#include <lua.h>
#define UV_STREAM_METATABLE_NAME "uv.uv_stream_t"

// tcp
int luv_tcp_new(lua_State* L);
int luv_tcp_bind(lua_State* L);
int luv_tcp_connect(lua_State* L);
int luv_tcp_nodelay(lua_State* L);
int luv_tcp_keepalive(lua_State* L);
int luv_tcp_getsockname(lua_State* L);
int luv_tcp_getpeername(lua_State* L);

// stream
int luv_listen(lua_State* L);
int luv_accept(lua_State* L);
int luv_read_start(lua_State* L);
int luv_read_stop(lua_State* L);
int luv_write(lua_State* L);
int luv_close(lua_State* L);
int luv_shutdown(lua_State* L);

/*
// udp
int luv_udp_new(lua_State* L);
int luv_udp_bind(lua_State* L);
int luv_udp_set_membership(lua_State* L);
int luv_udp_getsockname(lua_State* L);
int luv_udp_send(lua_State* L);
int luv_udp_recv_start(lua_State* L);
int luv_udp_recv_stop(lua_State* L);
int luv_udp_set_broadcast(lua_State* L);
int luv_udp_set_ttl(lua_State* L);
int luv_udp_set_multicast_ttl(lua_State* L);
int luv_udp_set_multicast_loopback(lua_State* L);
*/

#endif /* LUV_SOCKET */
