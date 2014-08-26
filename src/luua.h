#ifndef LUUA
#define LUUA

#include <lua.h>
#include <uv.h>

int luua_loop_init(lua_State* L);
int luua_loop_run(lua_State* L);

#endif /* LUUA */
