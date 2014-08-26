#ifndef LUUA_TIMER
#define LUUA_TIMER

#include "luua.h"

int luua_timer(lua_State* L);
int luua_timer_start(lua_State* L);
int luua_timer_stop(lua_State* L);
int luua_timer_again(lua_State* L);
int luua_timer_set_repeat(lua_State* L);
int luua_timer_get_repeat(lua_State* L);
int luua_timer_get_active(lua_State* L);

#endif /* LUUA_TIMER */
