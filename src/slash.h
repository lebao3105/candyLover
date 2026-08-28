#pragma once
#include <discord.h>

void create_game_slash_commands(struct discord *client);
void on_game_interaction_create(struct discord *client, const struct discord_interaction *interaction);

void create_ban_slash_command(struct discord *client);
void on_ban_interaction_create(struct discord *client, const struct discord_interaction *interaction);
