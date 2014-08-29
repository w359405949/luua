#ifndef LUUA_UTILS
#define LUUA_UTILS

void luua_stackDump(lua_State* L);
void luua_setgcmetamethod(lua_State* L, const char* name, lua_CFunction gc);

#endif /* LUUA_UTILS */
