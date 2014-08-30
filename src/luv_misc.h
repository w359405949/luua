#ifndef LUUA
#define LUUA

#include <lua.h>
#include <uv.h>

#define UV_LOOP_METATABLE_NAME "uv.uv_loop_t"

int luv_loop_new(lua_State* L);
int luv_run(lua_State* L);
int luv_loop_stop(lua_State* L);

#endif /* LUUA */
