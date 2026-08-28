#include "dsl/dsl.h"

#include <assert.h>
#include <libintl.h>
#include <lauxlib.h>
#include <logmod.h>

scopes_t SCOPE = UNSET;
struct lua_State *Lua = nullptr;

int include(struct lua_State *L)
{
    int result = 0; // 0 = fail, 1 = success

    WARN_TOO_MANY_ARGS(1, include);
    else if (lua_gettop(L) == 0)
    {
        luaL_error(L, _("include() needs ONE argument!"), nullptr);
        goto done;
    }

    const char *path = luaL_checkstring(L, 1);
    if (path == nullptr || strlen(path) == 0)
    {
        luaL_error(L, _("Empty path"), nullptr);
        goto done;
    }

    result = DSL_load_file(path);
    goto done;

done:
    lua_pushboolean(L, result);
    return 1;
}

int end_section(struct lua_State *L)
{
    if (SCOPE == UNSET)
        luaL_error(L, _("This ends nothing!"));
    else
        SCOPE = UNSET;
    return 0;
}

#define START_SECTION_IMPL(name, scope)   \
int start_## name (struct lua_State *L)   \
{                                         \
    WARN_TOO_MANY_ARGS(0, start_## name); \
    SCOPE = scope;                        \
    return 0;                             \
}

START_SECTION_IMPL(environment, ENV)
START_SECTION_IMPL(trapboy, TRAPBOY)
START_SECTION_IMPL(counter, COUNTER)
START_SECTION_IMPL(wordchain, WORDCHAIN)
START_SECTION_IMPL(wordhunt, WORDHUNT)

void DSL_global_init(lua_State *L)
{
    REGISTER_C_FUNCTION(include);
    REGISTER_C_FUNCTION(end_section);
    lua_register(L, "environment", &start_environment);
    lua_register(L, "trapboy", &start_trapboy);
    lua_register(L, "counter", &start_counter);
    lua_register(L, "wordchain", &start_wordchain);
    lua_register(L, "wordhunt", &start_wordhunt);
}

bool DSL_load_file(const char *path)
{
    logmod_log(INFO, nullptr, "Reading %s", path);
    if (luaL_loadfile(Lua, path) == LUA_OK)
    {
        if (lua_pcall(Lua, 0, LUA_MULTRET, 0) == LUA_OK)
        {
            logmod_log(INFO, nullptr, "Successfully read %s", path);
            return true;
        }
        else
            logmod_log(ERROR, nullptr, "Error running %s: %s", path, lua_tostring(Lua, -1));
    }
    else
        logmod_log(ERROR, nullptr, "Loading %s failed: %s", path, lua_tostring(Lua, -1));
    lua_pop(Lua, 1);
    return false;
}
