#pragma once

#define DISCORD_MSG_EVENT_ARGS \
    struct discord *client, const struct discord_message *event

#define DISCORD_INT_EVENT_ARGS \
    struct discord *client, const struct discord_interaction *event

// Allocates a new string using passed format and format arguments.
// If the string creation fails, the result will be either nullptr
// or an assertion. Otherwise, be sure to free() the result.
const char *vFormat(const char *fmt, ...);
