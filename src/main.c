#include <curl/curl.h> // curl_version
#include <stdlib.h> // at_exit
#include <stdio.h> // printf
#include <assert.h>
#include <getopt.h>
#include <libintl.h> // _()

#include <discord.h>
#include <logmod.h>
#include <discord-events.h>
#include <discord_codecs.h>

#include "dsl/dsl.h"
#include "dsl/env.h"
#include "slash.h"
#include "features/count.h"
#include <lua.h> // LUA_VERSION, lua_close
#include <lualib.h> // luaopen_*
#include <lauxlib.h> // luaL_newstate

char *env_path = "env.lua";
struct discord *discordClient = nullptr;

void on_ready(struct discord *client,
              const struct discord_ready *event)
{
    logmod_log(INFO, nullptr, "Hello world from %s#%s!",
               event->user->username, event->user->discriminator);
    create_game_slash_commands(client);
}

void onMessage(struct discord *client, const struct discord_message *event)
{
    if (event->interaction || event->author->bot) return;
    if (event->channel_id == number_count)
        on_count_message(client, event);
}

void on_exit()
{
    if (discordClient != nullptr)
        discord_cleanup(discordClient);

    if (Lua != nullptr)
        lua_close(Lua);
}

int main(int argc, char *argv[])
{
    const struct option options[] = {
        {"env-path", optional_argument, 0, 'e'},
        {"help",     no_argument,       0, 'h'},
        {"version",  no_argument,       0, 'v'},
        {0,          0,                 0, 0}
    };

    if (argc > 1) {
        for (;;) {
            int option_index = 0;
            char c = getopt_long(argc, argv, "e:d:hv", options, &option_index);
            if (c == -1) break;

            switch (c)
            {
            case 'h':
                printf(_("Bot Discord :white_check_mark:\nCờ khả dụng:\n"));
                printf(_("--env-path / -e [THƯ MỤC]       Đọc [THƯ MỤC] để setup các lệnh\n"));
                printf(_("--help     / -h                 In ra những dòng này\n"));
                printf(_("--version  / -v                 In ra phiên bản của bot+trình biên dịch v.v..\n"));
                printf(_("Ghé qua soc-bot(3) để biết thêm chi tiết."));
                return 0;
            case 'v':
                printf(_("Phiên bản API Discord: %s\n"), DISCORD_VERSION);
                printf(_("Phiên bản libcURL: %s\n"), curl_version());
                // LUA_VERSION expands to Lua + Lua version.
                // If that changes, fix the line.
                printf(_("Phiên bản %s"), LUA_VERSION"\n");
                return 0;
            case 'e':
                env_path = optarg;
                break;
            }
        }
    }

    atexit(&on_exit);

    Lua = luaL_newstate();
    assert((Lua != nullptr) && _("Failed to initialize Lua"));
    luaopen_base(Lua); luaopen_math(Lua);
    luaopen_string(Lua); luaopen_table(Lua);

    DSL_global_init(Lua);
    DSL_env_init(Lua);

    DSL_load_file(env_path);

    struct discord_config cfg = DSL_env_get_config();
    discordClient = discord_from_config(&cfg);
    assert(discordClient != nullptr && _("Couldn't initialize the client!"));

    discord_set_on_ready(discordClient, &on_ready);
    discord_set_on_message_create(discordClient, &onMessage);
    
    discord_run(discordClient);
    // Cleanup tasks are already done by on_exit()
    return 0;
}
