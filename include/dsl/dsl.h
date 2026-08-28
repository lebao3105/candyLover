#pragma once

#include "types.h"
#include <lua.h>

#define ASSERT_IN_SCOPE(name) \
    if (!SCOPE) \
        luaL_error(L, "This function is called outside " #name " scope", nullptr)

#define REGISTER_C_FUNCTION(name) \
    lua_register(L, #name, & name);

#define WARN_TOO_MANY_ARGS(n, name) \
    if (lua_gettop(L) > n) \
        lua_warning(L, #name "() needs only " #n " arguments", 0)


typedef enum { UNSET, ENV, COUNTER, TRAPBOY, WORDHUNT, WORDCHAIN } scopes_t;
extern scopes_t SCOPE;
extern struct lua_State *Lua;

bool DSL_load_file(const char *path);
void DSL_global_init(struct lua_State *L);
u64snowflake DSL_set_integer(struct lua_State *L);
