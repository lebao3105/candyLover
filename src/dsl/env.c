#include "dsl/dsl.h"
#include "dsl/env.h"
#include "swiss.h"

#include <assert.h>
#include <ctype.h> // toupper
#include <stdlib.h>
#include <string.h>
#include <lauxlib.h>
#include <libintl.h>
#include <logmod.h>

static char *token = nullptr;
static enum logmod_levels log_level = LOGMOD_LEVEL_TRACE;
u64snowflake botID,
             number_count,
             word_by_word,
             trapboy,
             modlogs
             = 0;

int set_token(struct lua_State *L)
{
    ASSERT_IN_SCOPE(env);
    WARN_TOO_MANY_ARGS(1, set_token);
    token = luaL_checkstring(L, 1);
    if (strlen(token) == 0) {
        luaL_argerror(L, 1, "Xâu rỗng???");
        logmod_log(FATAL, nullptr, "Empty token spotted");
    }
    return 0;
}

int set_log_level(struct lua_State *L)
{
    ASSERT_IN_SCOPE(env);
    WARN_TOO_MANY_ARGS(1, set_log_level);

    char *label = luaL_checkstring(L, 1);
    if (strlen(token) == 0)
    {
        luaL_argerror(L, 1, "Xâu rỗng???");
        logmod_log(FATAL, nullptr, "Empty token spotted");
    }
    else
    {
        for (char *it = label; *it; ++it)
            *it = (char)toupper((int)*it);
        auto lv = logmod_logger_get_level(nullptr, label);
        if (lv < 0)
            luaL_argerror(L, 1, "Invalid logging level");
        else
            log_level = lv;
    }
    return 0;
}

u64snowflake DSL_set_integer(struct lua_State *L)
{
    switch (lua_type(L, 1))
    {
        case LUA_TNUMBER:
            return lua_tointeger(L, 1);
        case LUA_TSTRING:
            const char *id = luaL_checkstring(L, 1);
            if (strlen(id) == 0) {
                luaL_argerror(L, 1, _("Empty string!?"));
                logmod_log(FATAL, nullptr, "Empty channel ID spotted");
                return 0;
            }

            char *errptr;
            u64snowflake result = strtoul(id, &errptr, 0);
            if (*errptr != '\0') {
                const char *msg = vFormat(
                    _("Invalid string value: not convertible to unsigned number (error message: %s)"),
                    errptr
                );
                luaL_argerror(L, 1, msg);
                free(msg);
            }
            return result;
        default:
            luaL_argerror(L, 1, _("Invalid value type: must be a string or an unsigned integer"));
            return 0;
    }
}

#define SET_CHANNEL_ID_IMPL(name)                                        \
int set_## name (struct lua_State *L)                                    \
{                                                                        \
    ASSERT_IN_SCOPE(env);                                                \
    WARN_TOO_MANY_ARGS(1, set_## name);                                  \
    name = DSL_set_integer(L);                                           \
    logmod_log(INFO, nullptr, "Successfully set channel ID for " #name); \
    return 0;                                                            \
}

int set_app_id(struct lua_State *L)
{
    ASSERT_IN_SCOPE(env);
    WARN_TOO_MANY_ARGS(1, set_app_ID);
    botID = DSL_set_integer(L);
    logmod_log(INFO, nullptr, "Successfully set application ID");
    return 0;
}

SET_CHANNEL_ID_IMPL(number_count)
SET_CHANNEL_ID_IMPL(word_by_word)
SET_CHANNEL_ID_IMPL(trapboy)
SET_CHANNEL_ID_IMPL(modlogs)
#undef SET_CHANNEL_ID_IMPL

void DSL_env_init(lua_State *L)
{
    REGISTER_C_FUNCTION(set_token);
    REGISTER_C_FUNCTION(set_app_id);
    REGISTER_C_FUNCTION(set_log_level);
    REGISTER_C_FUNCTION(set_number_count);
    REGISTER_C_FUNCTION(set_word_by_word);
    REGISTER_C_FUNCTION(set_trapboy);
    REGISTER_C_FUNCTION(set_modlogs);
}

struct discord_config DSL_env_get_config()
{
    assert(token != nullptr && "No token set!");
    return (struct discord_config) {
        .token = token,
        .log = {
            .level = log_level,
            .quiet = false,
            .color = true,
            .overwrite = false,
            .trace = fopen("trace.log", "a")
        }
    };
}
