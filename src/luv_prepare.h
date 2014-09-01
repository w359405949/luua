#ifndef LUV_PREPARE
#define LUV_PREPARE
#include <lua.h>
#define UV_PREPARE_METATABLE_NAME "uv.uv_prepare_t"

int luv_prepare_new(lua_State* L);
int luv_prepare_start(lua_State* L);
int luv_prepare_stop(lua_State* L);

#endif /* LUV_PREPARE */
