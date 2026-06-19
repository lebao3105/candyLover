#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <getopt.h>

#include <discord.h>
#include <logmod.h>
#include <discord-events.h>
#include <discord_codecs.h>

#include "dsl/dsl.h"
#include "dsl/env.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

char *conf_path = "config/";
char *env_path = "env.lua";
struct discord *discordClient = nullptr;

void on_ready([[maybe_unused]] struct discord *client,
              const struct discord_ready *event)
{
    logmod_log(INFO, NULL, "Chào thế giới từ %s#%s!",
               event->user->username, event->user->discriminator);
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
        {"conf-dir", optional_argument, 0, 'd'},
        {"help",     no_argument,       0, 'h'},
        {"version",  no_argument,       0, 'v'},
        {0,          0,                 0, 0}
    };

    for (;;) {
        int option_index = 0;
        char c = getopt_long(argc, argv, "e:d:hv", options, &option_index);
        if (c == -1) break;

        switch (c)
        {
        case 'h':
            printf("Bot Discord :white_check_mark:\nCờ khả dụng:\n");
            printf("--env-path / -e [TỆP]           Đọc [TỆP] để setup môi trường (API token v.v)\n"
                   "--conf-dir / -d [THƯ MỤC]       Đọc [THƯ MỤC] để setup các lệnh\n"
                   "--help     / -h                 In ra những dòng này\n"
                   "--version  / -v                 In ra phiên bản của bot+trình biên dịch v.v..\n"
                   "Ghé qua soc-bot(3) để biết thêm chi tiết.");
            return 0;
        case 'v':
            printf("Phiên bản API Discord: %s\n", DISCORD_VERSION);
            printf("Phiên bản libcURL: %s\n", curl_version());
            printf(LUA_VERSION"\n");
            return 0;
        case 'e':
            env_path = optarg;
            break;
        case 'd':
            conf_path = optarg;
            break;
        };
    }

    atexit(&on_exit);

    Lua = luaL_newstate();
    assert((Lua != nullptr) && "Failed to initialize Lua");
    luaopen_base(Lua); luaopen_math(Lua);
    luaopen_string(Lua); luaopen_table(Lua);
    DSL_env_init(Lua);

    DSL_load_file(env_path);

    struct discord_config cfg = DSL_env_get_config();
    discordClient = discord_from_config(&cfg);
    assert(discordClient != nullptr && "Couldn't initialize the client!");

    discord_set_on_ready(discordClient, &on_ready);

    discord_run(discordClient);
    // Cleanup tasks are already done by on_exit()
    return 0;
}
