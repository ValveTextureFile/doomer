#include "args/builder.h"

#include "utils.h"

#include <stdlib.h>

static const char *resolve_iwad(const profile_t *profile, const config_t *config)
{
    if (profile && profile->iwad && profile->iwad[0] != '\0')
    {
        return profile->iwad;
    }
    if (config && config->default_iwad && config->default_iwad[0] != '\0')
    {
        return config->default_iwad;
    }

    return NULL;
}

static bool argv_builder_grow(builder_t *builder, size_t needed_extra)
{
    if (!builder)
        return false;

    size_t needed = builder->entries + needed_extra + 1; // +1 for NULL terminator later
    if (needed <= builder->cap)
        return true;

    size_t new_capacity = (builder->cap == 0) ? 8 : builder->cap * 2;
    while (new_capacity < needed)
        new_capacity *= 2;

    char **new_argv = realloc(builder->argv, new_capacity * sizeof(char *));
    if (!new_argv)
        return false;

    builder->argv = new_argv;
    builder->cap = new_capacity;
    return true;
}

bool builder_init(builder_t *builder)
{
    if (!builder)
    {
        log_error("builder", "Failed to init builder.");
        return false;
    }

    builder->argv = NULL;
    builder->entries = 0;
    builder->cap = 0;
    return true;
}

void builder_free(builder_t *builder)
{
    if (!builder)
        return;
    for (size_t i = 0; i < builder->entries; i++)
    {
        free(builder->argv[i]);
    }
    free(builder->argv);

    builder->argv = NULL;
    builder->entries = 0;
    builder->cap = 0;
}

bool builder_push(builder_t *builder, const char *entry)
{
    if (!entry || !builder)
    {
        log_error("builder", "Invalid builder_push args.");
        return false;
    }

    if (!argv_builder_grow(builder, 1))
    {
        log_error("builder", "Failed to grow builder.");
        return false;
    }

    char *copy = dstr(entry);
    if (!copy)
    {
        log_error("builder", "Failed to duplicated entry.");
        return false;
    }

    builder->argv[builder->entries++] = copy;
    log("builder", "Added new entry to argument builder.");
    return true;
}

bool builder_push2(builder_t *builder, const char *flag, const char *value)
{
    if (!builder || !flag || !value)
    {
        log_error("builder", "Invalid builder_push2 args.");
        return false;
    }

    if (!builder_push(builder, flag))
        return false;
    if (!builder_push(builder, value))
        return false;

    return true;
}

bool builder_finalize(builder_t *builder)
{
    if (!builder)
    {
        log_error("builder", "Invalid builder_finalize arg.");
        return false;
    }

    if (!argv_builder_grow(builder, 0))
    {
        log_error("builder", "Failed to finalize argv builder.");
        return false;
    }

    builder->argv[builder->entries] = NULL;
    return true;
}

bool builder_total(builder_t *builder, const config_t *config, const profile_t *profile, const engine_spec_t *engine_spec)
{
    if (!builder || !profile || !engine_spec)
        return false;

    if (!builder_init(builder))
        return false;

    if (!builder_push(builder, engine_spec->exe_path))
        goto fail;

    const char *iwad = resolve_iwad(profile, config);
    if (iwad && engine_spec->supports_iwad)
    {
        if (!builder_push2(builder, "-iwad", iwad))
            goto fail;
    }

    if (engine_spec->supports_file && profile->mod_count > 0)
    {
        if (!builder_push(builder, "-file"))
            goto fail;

        for (size_t i = 0; i < profile->mod_count; i++)
        {
            if (!builder_push(builder, profile->mods[i]))
                goto fail;
        }
    }

    if (engine_spec->supports_deh && profile->deh_count > 0)
    {
        if (!builder_push(builder, "-deh"))
            goto fail;

        for (size_t i = 0; i < profile->deh_count; i++)
        {
            if (!builder_push(builder, profile->deh_files[i]))
                goto fail;
        }
    }

    if (profile->warp_map && profile->warp_map[0] != '\0')
    {
        switch (engine_spec->map_launch_style)
        {
        case MAP_LAUNCH_WARP:
            if (!builder_push2(builder, "-warp", profile->warp_map))
                goto fail;
            break;

        case MAP_LAUNCH_PLUS_MAP:
            if (!builder_push2(builder, "+map", profile->warp_map))
                goto fail;
            break;

        case MAP_LAUNCH_NONE:
        default:
            break;
        }
    }

    if (engine_spec->supports_skill)
    {
        char skillbuf[16];
        snprintf(skillbuf, sizeof(skillbuf), "%d", profile->skill);

        if (!builder_push2(builder, "-skill", skillbuf))
            goto fail;
    }

    if (profile->no_monsters)
    {
        if (!builder_push(builder, "-nomonsters"))
            goto fail;
    }

    if (profile->fast)
    {
        if (!builder_push(builder, "-fast"))
            goto fail;
    }

    if (profile->respawn)
    {
        if (!builder_push(builder, "-respawn"))
            goto fail;
    }

    for (size_t i = 0; i < profile->extra_arg_count; i++)
    {
        const char *arg = profile->extra_args[i];

        if (!engine_spec->supports_plus_commands && arg[0] != '\0' && arg[0] == '+')
            continue;

        if (!builder_push(builder, arg))
            goto fail;
    }

    if (!builder_finalize(builder))
        goto fail;

    return true;

fail:
    builder_free(builder);
    return false;
}