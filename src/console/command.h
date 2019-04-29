#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdbool.h>

#include "console.h"

typedef struct {
    const char *command;
    bool (*action)(const char *params[], int n);
} command_t;

command_t *find_command(const char *command);

#endif
