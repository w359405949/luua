#ifndef LUV_CHECK
#define LUV_CHECK
#include <lua.h>

int luv_check_new(lua_State* L);
int luv_check_start(lua_State* L);
int luv_check_stop(lua_State* L);

#endif /* LUV_CHECK */
