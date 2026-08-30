#pragma once
#include "swiss.h"
#include "types.h"
#include <discord.h>

struct ban_params {
    u64snowflake targetID;
    char *reason;
    bool kick;
};

void on_trap_trigger(DISCORD_MSG_EVENT_ARGS);
void on_ban_command(DISCORD_INT_EVENT_ARGS, struct ban_params *params);
void on_unban_command(DISCORD_INT_EVENT_ARGS, struct ban_params *params);