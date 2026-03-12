#ifndef BUILDER_H
#define BUILDER_H

#include <stddef.h>
#include <stdbool.h>

/// @brief Typedef builder type for building a passable argv.
typedef struct builder_t
{
    /// @brief Dynamic argv.
    char **argv;

    /// @brief Amount of argument entries.
    size_t entries;

    /// @brief Max capacity of entries.
    size_t cap;
} builder_t;

/// @brief Intialize a builder.
/// @param builder The builder to pass.
/// @return true on success, false on failure.
bool builder_init(builder_t* builder);

/// @brief Free a builder
/// @param builder The builder to pass.
void builder_free(builder_t* builder);

/// @brief Push a single flag to the builder.
/// @param builder The builder to pass.
/// @param entry The argument to pass
/// @return true on success, false on failure.
bool builder_push(builder_t* builder, const char* entry);

/// @brief Pushes two arguments to the builder, usually a flag with a value.
/// @param builder The builder to pass.
/// @param flag The argument.
/// @param value The value to pass to the argument.
/// @return true on success, false on failure.
bool builder_push2(builder_t* builder, const char* flag, const char* value);

/// @brief Finalize the builder. Adds a NULL terminator to the end.
/// @param builder The builder to pass.
/// @return true on success, false on failure.
bool builder_finalize(builder_t* builder);

#endif