#ifndef LUUA
#define LUUA

#include <lua.h>
#include <uv.h>

int luv_loop_new(lua_State* L);
int luv_run(lua_State* L);

#endif /* LUUA */
