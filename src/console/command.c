#ifndef _COMMAND_
#define _COMMAND_

#include "command.h"

static bool pwd(const char *params[], int n);
static bool cd(const char *params[], int n);

static command_t command_list[] = {
    {"pwd", pwd},
    {"cd", cd}};

command_t *find_command(const char *str) {
    for (int i = 0; i < sizeof(command_list); i++) {
        if (strcmp(command_list[i].command, str) == 0) {
            return &command_list[i];
        }
    }

    return NULL;
}

static bool pwd(const char *params[], int n) {
    char cwd[4096];
    getcwd(cwd);
    console_println(cwd);

    return true;
}

static bool cd(const char *params[], int n) {
    if (n == 0) {
        chdir("/");
    } else {
        chdir(params[0]);
    }

    return true;
}

#endif
