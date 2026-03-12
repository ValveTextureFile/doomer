#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "lib/ansi.h"

/// @brief Stylized error log.
/// @param from Origin of error.
/// @param message Error message.
static void log_error(char *from, char *message) {
    console_set_color(CONSOLE_FG_BRIGHT_RED);
    printf("\aerror: [%s] %s\n", from, message);
    console_reset_color();
}

/// @brief Stylized log messaging.
/// @param from Origin.
/// @param message Message.
static void log(char *from, char *message) {
    printf("[%s] %s", from, message);
}

#endif