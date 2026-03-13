#include "data.h"

cfg_opt_t config_default_opts[] = {
    CFG_STR("default_iwad", "doom2.wad", CFGF_NONE),
    CFG_STR("default_engine", "zdoom", CFGF_NONE),
    CFG_STR("root_folder", "/.doomer", CFGF_NONE),
    CFG_END()
};

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
    CFG_END()
};

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
    CFG_END()
};