#include "slash.h"
#include "features/count.h"
#include "dsl/env.h"
#include <libintl.h>
#include <string.h>

#define LOGMOD_CONTEXT_ID "SLASH"
#include <logmod.h>

void create_game_slash_commands(struct discord *client)
{
    struct discord_application_command_option_choice names[] = {
        { .name = "counter", .value = "\"counter\"" }
    };
    struct discord_application_command_option_choice actions[] = {
        { .name = _("Modify"), .value = "\"mod\"" },
        { .name = _("Reset"), .value = "\"reset\"" },
        { .name = _("Start"), .value = "\"start\"" },
        { .name = _("Lock"), .value = "\"lock\""},
        { .name = _("Query infos"), .value = "\"query\"" }
    };
    struct discord_application_command_option options[] = {
        {
            .type = DISCORD_APPLICATION_OPTION_STRING,
            .name = _("name"),
            .description = _("Name of the game"),
            .required = true,
            .choices = &(struct discord_application_command_option_choices) {
                .size = sizeof(names) / sizeof *names,
                .array = names
            }
        },
        {
            .type = DISCORD_APPLICATION_OPTION_STRING,
            .name = _("action"),
            .description = _("What to do"),
            .choices = &(struct discord_application_command_option_choices){
                .size = sizeof actions / sizeof *actions,
                .array = actions
            },
            .required = true
        }
    };
    struct discord_create_global_application_command params = {
        .name = "game",
        .description = _("Query, start, reset or modify games"),
        .default_permission = true,
        .options = &(struct discord_application_command_options) {
            .size = sizeof(options) / sizeof *options,
            .array = options
        }
    };
    discord_create_global_application_command(client, botID, &params, nullptr);
    discord_set_on_interaction_create(client, &on_game_interaction_create);
    logmod_log(INFO, nullptr, "Created slash commands!");
}

void on_game_interaction_create(struct discord *client, const struct discord_interaction *interaction)
{
    if ((interaction->type != DISCORD_INTERACTION_APPLICATION_COMMAND) ||
        !interaction->data || !interaction->data->options) return;

    char *name = nullptr;
    char *action = nullptr;

    for (int i = 0; i < interaction->data->options->size; ++i)
    {
        char *nm = interaction->data->options->array[i].name;
        char *vl = interaction->data->options->array[i].value;

        if (strcmp(nm, _("name")) == 0)
            name = vl;
        else if (strcmp(nm, _("action")) == 0)
            action = vl;

        if (name && action) break;
    }

    if (strcmp(name, "counter") == 0) {
        if (strcmp(action, "query") == 0) {
            on_count_command(client, interaction);
        }
    }
}