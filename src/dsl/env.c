#include "dsl/dsl.h"
#include "dsl/env.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logmod.h>
#include <lauxlib.h>

CREATE_DSL_SCOPE_VAL;
static char *token = nullptr;
enum logmod_levels log_level = LOGMOD_LEVEL_TRACE;
u64snowflake number_count,
             word_by_word,
             trapboy,
             modlogs
             = 0;

int start_env_section(struct lua_State *L)
{
    WARN_TOO_MANY_ARGS(0, environment);
    SET_IN_SCOPE(true);
    return 0;
}

int end_env_section(struct lua_State *L)
{
    ASSERT_IN_SCOPE(env);
    WARN_TOO_MANY_ARGS(0, end_environment);
    SET_IN_SCOPE(false);
    return 0;
}

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

u64snowflake set_channel_id(struct lua_State *L)
{
    ASSERT_IN_SCOPE(env);
    WARN_TOO_MANY_ARGS(1, set_*_channel_ID);

    const char *id = luaL_checkstring(L, 1);
    if (strlen(id) == 0) {
        luaL_argerror(L, 1, "Xâu rỗng???");
        logmod_log(FATAL, nullptr, "Empty channel ID spotted");
    }
    char *errptr;
    auto result = strtoul(id, &errptr, 0);
    if (*errptr != '\0') {
        luaL_argerror(L, 1, "Channel ID không hợp lệ");
        logmod_log(FATAL, nullptr, "Invalid channel ID spotted (invalid characters: %s)", errptr);
    }
    return result;
}

#define SET_CHANNEL_ID_IMPL(name) \
int set_## name (struct lua_State *L) \
{ \
    name = set_channel_id(L); \
    logmod_log(INFO, nullptr, "Successfully set channel ID for " #name); \
    return 0; \
}

SET_CHANNEL_ID_IMPL(number_count)
SET_CHANNEL_ID_IMPL(word_by_word)
SET_CHANNEL_ID_IMPL(trapboy)
SET_CHANNEL_ID_IMPL(modlogs)

void DSL_env_init(lua_State *L)
{
    lua_register(L, "environment", &start_env_section);
    lua_register(L, "end_environment", &end_env_section);
    REGISTER_C_FUNCTION(set_token);
    REGISTER_C_FUNCTION(set_log_level);
    REGISTER_C_FUNCTION(set_number_count);
    REGISTER_C_FUNCTION(set_word_by_word);
    REGISTER_C_FUNCTION(set_trapboy);
    REGISTER_C_FUNCTION(set_modlogs);
}

void DSL_end_env_section()
{
    SET_IN_SCOPE(false);
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
