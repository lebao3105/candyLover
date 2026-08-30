#include "features/ban.h"
#include "discord_codecs.h"
#include <libintl.h>

void on_trap_trigger(DISCORD_MSG_EVENT_ARGS)
{
    if ((event->member->permissions & DISCORD_PERM_BAN_MEMBERS) != 0) return;

    struct discord_create_guild_ban params = {
        .delete_message_days = 1,
        .reason = _(
            "You have just stepped into a trap! If you believe that "
            "this is a mistake, find your way to talk to a moderator."
        )
    };

    discord_create_guild_ban(client, event->guild_id, event->author->id, &params, nullptr);
}

void on_ban_command(DISCORD_INT_EVENT_ARGS, struct ban_params *params)
{
    discord_create_guild_ban(
        client, event->guild_id, params->targetID,
        &(struct discord_create_guild_ban) {
            .delete_message_days = 1,
            .reason = params->reason
        }, nullptr
    );
}

void on_unban_command(DISCORD_INT_EVENT_ARGS, struct ban_params *params)
{
    discord_remove_guild_ban(
        client, event->guild_id, params->targetID,
        &(struct discord_remove_guild_ban) {
            .reason = params->reason
        }, nullptr
    );
}