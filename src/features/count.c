#include "features/count.h"
#include "discord_codecs.h"
#include "dsl/count.h"
#include "swiss.h"
#include <stdlib.h>
#include <libintl.h>
#include <lualib.h>
#include <lauxlib.h>

static lua_Integer currentPosition = 0;
static u64snowflake lastCounter = 0;

// TODO: Move
int on_error(struct lua_State *L)
{
    const char *errorMsg = lua_tostring(L,-1);
    luaL_traceback(L,L, errorMsg, 1);
    return 1;
}

void on_count_message(struct discord *client, const struct discord_message *event)
{
    struct discord_create_message params = {
        .message_reference = event->message_reference,
        .content = nullptr
    };
    // TODO: Reuse
    struct lua_State *L = nullptr;
    
    if (lastCounter == event->author->id) {
        params.content = _("You wanna count twice? Nah I'd win. Resetting the counter.");
        goto send;
    }
    
    // TODO: Null check
    L = luaL_newstate();
    luaopen_math(L);
    lua_pushcfunction(L, &on_error);

    const char *command = vFormat("return %s", event->content);
    if (luaL_dostring(L, command) != LUA_OK)
    {
        params.content = vFormat(
            _("Invalid input or an error occured: %s"),
            lua_tostring(L, -1)
        );
        free(command);
        goto send;
    }
    else if (lua_isinteger(L, -1) != 1) {
        params.content = _("The input is not convertible into an integer - resetting the counter");
        free(command);
        goto send;
    }

    free(command);
    const auto num = lua_tointeger(L, -1);
    if ((num - countStep) != currentPosition)
    {
        params.content = vFormat(
            _("The next number must be %u - resetting the counter"),
            currentPosition + 1
        );
        goto send;
        free(params.content);
    }

    ++currentPosition;
    // The emoji is :white_check_mark:
    discord_create_reaction(client, event->channel_id, event->id, 0, "\U00002705", nullptr);
    return;

send:
    if (L) lua_close(L);
    if (params.content != nullptr)
        discord_create_message(client, event->channel_id, &params, nullptr);
    // The emoji is :x:
    discord_create_reaction(client, event->channel_id, event->id, 0, "\U0000274C", nullptr);
}

void on_count_command(DISCORD_INT_EVENT_ARGS)
{
    const char *msg = vFormat(
        "Current position: %u\n"
        "Step: %u\n"
        "First number: %u\n",
        currentPosition,
        countStep,
        countStartingPoint + 1
    );

    struct discord_interaction_response params = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data){ .content = msg }
    };

    discord_create_interaction_response(client, event->id, event->token, &params, nullptr);
    free((void*)msg);
}