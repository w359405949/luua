#ifndef LUV_CHECK
#define LUV_CHECK
#include <lua.h>
#define UV_CHECK_METATABLE_NAME "uv.uv_check_t"

int luv_check_new(lua_State* L);
int luv_check_start(lua_State* L);
int luv_check_stop(lua_State* L);

#endif /* LUV_CHECK */
