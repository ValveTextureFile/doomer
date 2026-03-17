#include "data.h"

#include <stdlib.h>
#include <string.h>
#include "utils.h"

/// @brief Internal helper to free a copied string list.
/// @param list 
/// @param count 
static void free_string_list(char **list, size_t count)
{
    if (!list)
        return;

    for (size_t i = 0; i < count; i++)
    {
        free(list[i]);
    }

    free(list);
}

/// @brief Internal helper to copy a libConfuse string list.
/// @param cfg 
/// @param key 
/// @param out_list 
/// @param out_count 
/// @return true on success, false on failure.
static bool copy_cfg_string_list(cfg_t *cfg, const char *key, char ***out_list, size_t *out_count)
{
    size_t count = (size_t)cfg_size(cfg, key);
    char **list = NULL;

    if (count > 0)
    {
        list = calloc(count, sizeof(char *));
        if (!list)
        {
            return false;
        }

        for (size_t i = 0; i < count; i++)
        {
            const char *item = cfg_getnstr(cfg, key, (unsigned int)i);
            list[i] = dstr(item);
            if (!list[i])
            {
                free_string_list(list, i);
                return false;
            }
        }
    }

    *out_list = list;
    *out_count = count;
    return true;
}

/// @brief Free a profile.
/// @param profile 
void free_profile(profile_t *profile)
{
    if (!profile)
        return;

    free(profile->name);
    free(profile->path);
    free(profile->engine_id);
    free(profile->iwad);
    free(profile->warp_map);

    free_string_list(profile->mods, profile->mod_count);
    free_string_list(profile->deh_files, profile->deh_count);
    free_string_list(profile->extra_args, profile->extra_arg_count);

    memset(profile, 0, sizeof(*profile));
}

/// @brief Free a config.
/// @param config 
void free_config(config_t *config)
{
    if (!config)
        return;

    free(config->default_engine);
    free(config->root_folder);
    free(config->default_iwad);

    memset(config, 0, sizeof(*config));
}

/// @brief Free an engine spec.
/// @param engine_spec 
void free_engine_spec(engine_spec_t *engine_spec)
{
    if (!engine_spec)
        return;

    free(engine_spec->exe_path);
    free(engine_spec->id);
    free(engine_spec->name);

    memset(engine_spec, 0, sizeof(*engine_spec));
}

//////////////////////////////////////////

/// @brief Default configuration options for libConfuse.
cfg_opt_t config_default_opts[] = {
    CFG_STR("default_iwad", "", CFGF_NONE),
    CFG_STR("default_engine", "", CFGF_NONE),
    CFG_STR("root_folder", "", CFGF_NONE),
    CFG_END()};

/// @brief Default engine spec options for libConfuse.
cfg_opt_t engine_spec_default_opts[] = {
    CFG_STR("id", "", CFGF_NONE),
    CFG_STR("name", "", CFGF_NONE),
    CFG_STR("path", "", CFGF_NONE),
    CFG_STR("argstyle", "alien", CFGF_NONE),
    CFG_STR("map_launch_style", "warp", CFGF_NONE),
    CFG_BOOL("iwad_support", cfg_false, CFGF_NONE),
    CFG_BOOL("file_support", cfg_false, CFGF_NONE),
    CFG_BOOL("deh_support", cfg_false, CFGF_NONE),
    CFG_BOOL("skill_support", cfg_false, CFGF_NONE),
    CFG_BOOL("merge_support", cfg_false, CFGF_NONE),
    CFG_BOOL("response_files_support", cfg_false, CFGF_NONE),
    CFG_BOOL("plus_commands_support", cfg_false, CFGF_NONE),
    CFG_END()};

/// @brief Default profile options for libConfuse.
cfg_opt_t profile_default_opts[] = {
    CFG_STR("name", "", CFGF_NONE),
    CFG_STR("engine_id", "", CFGF_NONE),
    CFG_STR("iwad", "", CFGF_NONE),
    CFG_STR_LIST("mods", 0, CFGF_NONE),
    CFG_STR_LIST("deh", 0, CFGF_NONE),
    CFG_STR("warp_map", "", CFGF_NONE),
    CFG_INT("skill", 1, CFGF_NONE),
    CFG_BOOL("no_monsters", cfg_false, CFGF_NONE),
    CFG_BOOL("fast_monsters", cfg_false, CFGF_NONE),
    CFG_BOOL("monster_respawn", cfg_false, CFGF_NONE),
    CFG_STR_LIST("extra", 0, CFGF_NONE),
    CFG_END()};

////////////////////////////////////////////////////////////////////////////////////

/// @brief Load profile model from path.
/// @param path Path to profile file (.dmp)
/// @return profile_t.
profile_t lf_profile(const char *path)
{
    profile_t profile = {0};
    memset(&profile, 0, sizeof(profile));

    if (!path)
    {
        return profile;
    }

    cfg_t *cfg = cfg_init(profile_default_opts, CFGF_NONE);
    if (!cfg)
    {
        return profile;
    }

    if (cfg_parse(cfg, path) != CFG_SUCCESS)
    {
        cfg_free(cfg);
        return profile;
    }

    profile.name = dstr(cfg_getstr(cfg, "name"));
    profile.path = dstr(path);
    profile.engine_id = dstr(cfg_getstr(cfg, "engine_id"));
    profile.iwad = dstr(cfg_getstr(cfg, "iwad"));
    profile.warp_map = dstr(cfg_getstr(cfg, "warp_map"));
    profile.skill = cfg_getint(cfg, "skill");

    profile.no_monsters = cfg_getbool(cfg, "no_monsters");
    profile.fast = cfg_getbool(cfg, "fast_monsters");
    profile.respawn = cfg_getbool(cfg, "monster_respawn");

    if (!copy_cfg_string_list(cfg, "mods", &profile.mods, &profile.mod_count) ||
        !copy_cfg_string_list(cfg, "deh", &profile.deh_files, &profile.deh_count) ||
        !copy_cfg_string_list(cfg, "extra", &profile.extra_args, &profile.extra_arg_count))
    {
        cfg_free(cfg);
        free_profile(&profile);
        return (profile_t){0};
    }

    printf("[parser] loaded profile %s:\n", path);
    cfg_print(cfg, stdout);

    cfg_free(cfg);

    return profile;
}

/// @brief Load config model from path.
/// @param path Path to config file (.dconf)
/// @return 
config_t lf_config(const char *path)
{
    config_t config = {0};
    memset(&config, 0, sizeof(config));

    if (!path)
        return config;

    cfg_t *cfg = cfg_init(config_default_opts, CFGF_NONE);
    if (!cfg)
        return config;

    if (cfg_parse(cfg, path) == CFG_PARSE_ERROR)
    {
        cfg_free(cfg);
        return config;
    }

    config.default_engine = dstr(cfg_getstr(cfg, "default_engine"));
    config.default_iwad = dstr(cfg_getstr(cfg, "default_iwad"));
    config.root_folder = dstr(cfg_getstr(cfg, "root_folder"));

    printf("[parser] loaded config file %s:\n", path);
    cfg_print(cfg, stdout);

    cfg_free(cfg);
    return config;
}

/// @brief Load engine spec model from path.
/// @param path Path to engine spec file (.dme)
/// @return 
engine_spec_t lf_engine_spec(const char *path)
{
    engine_spec_t engine_spec = {0};
    if (!path)
        return engine_spec;

    cfg_t *cfg = cfg_init(engine_spec_default_opts, CFGF_NONE);
    if (!cfg)
        return engine_spec;

    if (cfg_parse(cfg, path) == CFG_PARSE_ERROR)
    {
        cfg_free(cfg);
        return engine_spec;
    }

    engine_spec.exe_path = dstr(cfg_getstr(cfg, "path"));
    engine_spec.id = dstr(cfg_getstr(cfg, "id"));
    engine_spec.name = dstr(cfg_getstr(cfg, "name"));

    {
        const char *argstyle = cfg_getstr(cfg, "argstyle");
        if (argstyle == NULL)
        {
            engine_spec.argstyle = 0;
        }
        else if (strcmp(argstyle, "alien") == 0)
        {
            engine_spec.argstyle = ARGSTYLE_ALIEN;
        }
        else if (strcmp(argstyle, "eternity") == 0)
        {
            engine_spec.argstyle = ARGSTYLE_ETERNITY;
        }
        else if (strcmp(argstyle, "zdoom") == 0)
        {
            engine_spec.argstyle = ARGSTYLE_ZDOOM;
        }
        else if (strcmp(argstyle, "vanilla") == 0)
        {
            engine_spec.argstyle = ARGSTYLE_VANILLA;
        }
        else
        {
            engine_spec.argstyle = 0;
        }
    }

    {
        const char *map_launch_style = cfg_getstr(cfg, "map_launch_style");
        if (map_launch_style == NULL)
        {
            engine_spec.map_launch_style = 0;
        }
        else if (strcmp(map_launch_style, "none") == 0)
        {
            engine_spec.map_launch_style = MAP_LAUNCH_NONE;
        }
        else if (strcmp(map_launch_style, "warp") == 0)
        {
            engine_spec.map_launch_style = MAP_LAUNCH_WARP;
        }
        else if (strcmp(map_launch_style, "map") == 0 || strcmp(map_launch_style, "plusmap") == 0)
        {
            engine_spec.map_launch_style = MAP_LAUNCH_PLUS_MAP;
        }
        else
        {
            engine_spec.map_launch_style = 0;
        }
    }

    engine_spec.supports_deh = cfg_getbool(cfg, "deh_support");
    engine_spec.supports_file = cfg_getbool(cfg, "file_support");
    engine_spec.supports_iwad = cfg_getbool(cfg, "iwad_support");
    engine_spec.supports_merge = cfg_getbool(cfg, "merge_support");
    engine_spec.supports_plus_commands = cfg_getbool(cfg, "plus_commands_support");
    engine_spec.supports_response_files = cfg_getbool(cfg, "response_files_support");
    engine_spec.supports_skill = cfg_getbool(cfg, "skill_support");

    printf("[parser] loaded engine spec %s:\n", path);
    cfg_print(cfg, stdout);

    cfg_free(cfg);
    return engine_spec;
}