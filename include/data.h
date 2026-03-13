#ifndef DATA_H
#define DATA_H

#include <confuse.h>
#include <stdbool.h>

extern cfg_opt_t config_default_opts[];
extern cfg_opt_t engine_spec_default_opts[];
extern cfg_opt_t profile_default_opts[];

bool init_config(cfg_t *cfg);
bool init_profile(cfg_t *profile);
bool init_engine_spec(cfg_t *engine_spec);

#endif