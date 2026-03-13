#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN64
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
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

// NOTE: Something to comeback to later.
// #ifdef _WIN64   // Windows
// static void alert(char* message) {
//     MessageBox(
//         NULL,                            
//         message, 
//         "doomer",         
//         MB_OK | MB_ICONINFORMATION
//     );
// }
// #elifdef __APPLE__ // OSX
// static void alert(char* message) {
//     char main_msg[256] = "osascript -e 'tell app \"Finder\" to display dialog \"%s\" buttons {\"OK\"}'";
//     snprintf(main_msg, sizeof(main_msg), message);
//     system(main_msg);
// }
// #else // Just wrap the log
// static void alert(char* message){
//     log("ALERT", message);
// }
// #endif

// 
#endif // UTILS_H