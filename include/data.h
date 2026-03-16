#ifndef DATA_H
#define DATA_H

#include <confuse.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"

extern cfg_opt_t config_default_opts[];
extern cfg_opt_t engine_spec_default_opts[];
extern cfg_opt_t profile_default_opts[];

profile_t lf_profile(const char *path);
config_t lf_config(const char *path);
engine_spec_t lf_engine_spec(const char *path);

void free_profile(profile_t* profile);
void free_config(config_t* config_t);
void free_engine_spec(engine_spec_t* engine_spec);

#endif