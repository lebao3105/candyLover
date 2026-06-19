#pragma once

#include <lua.h>

#define CREATE_DSL_SCOPE_VAL \
    static bool SCOPE = false

#define ASSERT_IN_SCOPE(name) \
    if (!SCOPE) \
        luaL_error(L, "This function is called outside " #name " scope", nullptr)

#define SET_IN_SCOPE(val) SCOPE = val

#define REGISTER_C_FUNCTION(name) \
    lua_register(L, #name, & name);

#define WARN_TOO_MANY_ARGS(n, name) \
    if (lua_gettop(L) > n) \
        lua_warning(L, #name "() needs only " #n " arguments", 0)

extern char *scopes[];
extern struct lua_State *Lua;

void DSL_load_file(const char *path);
void DSL_global_init(struct lua_State *L);
