#include <lauxlib.h>
#include "luv.h"
#include "luv_timer.h"
#include "luv_check.h"
#include "luv_misc.h"

#include "utils.h"

static const struct luaL_Reg luv[] = {

    // misc
    {"uv_loop_new", luv_loop_new},
    {"uv_run", luv_run},

    // timer
    {"uv_timer_new", luv_timer_new},
    {"uv_timer_start", luv_timer_start},
    {"uv_timer_stop", luv_timer_stop},
    {"uv_timer_again", luv_timer_again},
    {"uv_timer_set_repeat", luv_timer_set_repeat},
    {"uv_timer_get_repeat", luv_timer_get_repeat},

    // check
    {"uv_check_new", luv_check_new},
    {"uv_check_start", luv_check_start},
    {"uv_check_stop", luv_check_stop},

    {NULL, NULL}
};


int luaopen_libluv(lua_State* L)
{
    lua_newtable(L);
    luaL_setfuncs(L, luv, 0);
    return 1;
}
