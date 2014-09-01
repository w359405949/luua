#ifndef LUV
#define LUV
#include <lua.h>
#include "luv_timer.h"
#include "luv_check.h"
#include "luv_prepare.h"
#include "luv_misc.h"

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

    // prepare
    {"uv_prepare_new", luv_prepare_new},
    {"uv_prepare_start", luv_prepare_start},
    {"uv_prepare_stop", luv_prepare_stop},

    {NULL, NULL}
};


int luaopen_luv(lua_State* L);

#endif /* LUV */
