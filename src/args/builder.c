#include "args/builder.h"

#include "utils.h"

#include <stdlib.h>

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