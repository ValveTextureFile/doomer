#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>
#include <stddef.h>

/// @brief Typedef model for the config file.
typedef struct config_t
{
    /// @brief Default IWAD used if a profile does not specify one.
    char *default_iwad;

    /// @brief Default engine used if a profile does not specify one.
    char *default_engine;

    /// @brief Main folder that contains mods, iwads, profiles, and engines.
    char *root_folder;
} config_t;

/// @brief Typedef model for launcher paths.
/// These paths stem from the root folder and point to key directories.
typedef struct paths_t
{
    /// @brief Main root path for the launcher.
    char *root_path;

    /// @brief Stems from the root path. Directs to the "iwads" folder.
    char *iwads_path;

    /// @brief Stems from the root path. Directs to the "mods" folder.
    char *mods_path;

    /// @brief Stems from the root path. Directs to the "profiles" folder.
    char *profiles_path;

    /// @brief Stems from the root path. Directs to the "engines" folder.
    char *engines_path;
} paths_t;

/// @brief Typedef model for argstyles, which vary across engines.
typedef enum argstyle_t
{
    /// @brief Vanilla-ish argstyle baseline.
    /// Supported by ChocolateDOOM, Crispy, DSDA, and PrBoom-family ports.
    ARGSTYLE_VANILLA,

    /// @brief ZDoom-family argstyle.
    /// Supported by GZDoom, LZDoom, Zandronum, etc.
    ARGSTYLE_ZDOOM,

    /// @brief Eternity-specific argstyle.
    /// Supports additional behaviors like response files and loose loading.
    ARGSTYLE_ETERNITY,

    /// @brief Unknown or custom argstyles.
    /// @warning Args must be defined by the user.
    ARGSTYLE_ALIEN
} argstyle_t;

/// @brief Map launch styles.
/// Determines how the launcher should instruct the engine to start a map.
typedef enum map_launch_style_t
{
    /// @brief Engine does not support launching directly into a map.
    MAP_LAUNCH_NONE,

    /// @brief Engine uses the classic "-warp" flag.
    MAP_LAUNCH_WARP,

    /// @brief Engine uses "+map" console command style.
    MAP_LAUNCH_PLUS_MAP
} map_launch_style_t;

/// @brief Typedef model for specifications of engines.
typedef struct engine_spec_t
{
    /// @brief ID of engine.
    char *id;

    /// @brief Display name of the engine.
    char *name;

    /// @brief Path to the engine executable.
    char *exe_path;

    /// @brief Argstyle of the engine.
    argstyle_t argstyle;

    /// @brief Determines how maps are launched for this engine.
    map_launch_style_t map_launch_style;

    /// @brief This should be enabled for every engine,
    /// unless the engine has an IWAD built-in.
    bool supports_iwad;

    /// @brief Supports loading mods via "-file".
    bool supports_file;

    /// @brief Supports DeHackEd patches.
    bool supports_deh;

    /// @brief Supports setting skill from command line.
    bool supports_skill;

    /// @brief Supports merging WADs.
    /// Used mainly by Chocolate Doom.
    bool supports_merge;

    /// @brief Supports response files (@args.txt style).
    bool supports_response_files;

    /// @brief Supports plus commands (+map, +set, etc).
    bool supports_plus_commands;
} engine_spec_t;

/// @brief Typedef model for user-made profiles.
typedef struct profile_t
{
    /// @brief Name of the profile.
    char *name;

    /// @brief Direct path to profile file (*.dmp).
    /// @note File format will be determined later.
    char *path;

    /// @brief Engine ID used for this profile.
    char *engine_id;

    /// @brief Direct path to IWAD.
    /// May reference default IWAD or a custom one.
    char *iwad;

    /// @brief Array of mods as their paths.
    /// Supported formats: wad / pk3 / pke / zip
    char **mods;

    /// @brief Number of mods loaded.
    size_t mod_count;

    /// @brief DeHackEd files used by the profile.
    char **deh_files;

    /// @brief Number of DeHackEd files.
    size_t deh_count;

    /// @brief Map to warp to.
    /// Examples: "MAP01", "E1M1"
    char *warp_map;

    /// @brief Skill level.
    /// Engine may ignore if unsupported.
    int skill;

    /// @brief No monsters toggle.
    bool no_monsters;

    /// @brief Fast monsters toggle.
    bool fast;

    /// @brief Monster respawn toggle.
    bool respawn;

    /// @brief Extra raw arguments passed directly to engine.
    /// Used for unsupported or experimental flags.
    char **extra_args;

    /// @brief Number of extra arguments.
    size_t extra_arg_count;
} profile_t;

#endif