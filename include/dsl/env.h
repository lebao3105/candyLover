#pragma once
#include <lua.h>
#include <discord.h>

void DSL_env_init(struct lua_State *L);
struct discord_config DSL_env_get_config();

extern
u64snowflake botID,
             number_count,
             word_by_word,
             trapboy,
             modlogs
             ;
