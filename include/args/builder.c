#include "builder.h"

#include "../utils.h"

#include <stdlib.h>

static bool argv_builder_grow(builder_t* builder, size_t needed_extra)
{
    if (!builder)
        return false;

    size_t needed = builder -> entries + needed_extra + 1; // +1 for NULL terminator later
    if (needed <= builder -> cap)
        return true;

    size_t new_capacity = (builder -> cap == 0) ? 8 : builder -> cap * 2;
    while (new_capacity < needed)
        new_capacity *= 2;

    char **new_argv = realloc(builder -> argv, new_capacity * sizeof(char *));
    if (!new_argv)
        return false;

    builder -> argv = new_argv;
    builder -> cap = new_capacity;
    return true;
}

bool builder_init(builder_t* builder) {
    if (!builder) {
        log_error("builder", "Failed to init builder.");
        return false;
    }

    builder -> argv = NULL;
    builder -> entries = 0;
    builder -> cap = 0;
    return true;
}

void builder_free(builder_t* builder) {
    
}