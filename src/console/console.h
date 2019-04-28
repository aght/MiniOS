#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdarg.h>
#include <stdbool.h>

#include "../hal/hal.h"

#include "../drivers/stdio/emb-stdio.h"

typedef struct {
    const char* command;
    bool (*action)(char);
} command_t;

void console_init();
void console_run();

void cd(const char* dir);

int console_print(const char *fmt, ...);
int console_println(const char *fmt, ...);
int console_newline();

#endif
