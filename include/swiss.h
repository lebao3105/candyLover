#pragma once

#define DISCORD_MSG_EVENT_ARGS \
    struct discord *client, const struct discord_message *event

#define DISCORD_INT_EVENT_ARGS \
    struct discord *client, const struct discord_interaction *event
