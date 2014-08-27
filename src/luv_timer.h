#ifndef LUV_TIMER
#define LUV_TIMER
#include <lua.h>

int luv_timer_new(lua_State* L);
int luv_timer_start(lua_State* L);
int luv_timer_stop(lua_State* L);
int luv_timer_again(lua_State* L);
int luv_timer_set_repeat(lua_State* L);
int luv_timer_get_repeat(lua_State* L);

#endif /* LUV_TIMER */
