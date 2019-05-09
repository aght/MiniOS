#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../drivers/stdio/emb-stdio.h"
#include "../hal/hal.h"
#include "../coreutils/coreutils.h"
#include "command.h"
#include "ascii_colors.h"

#define COMMAND_SUCCESS 0x2
#define COMMAND_CLEAR 0x4
#define COMMAND_FAILURE 0x8
#define COMMAND_NOT_FOUND 0x10

void console_init();
void console_run();

char* getcwd(char buf[]);
void chdir(const char *dir);
void console_clear();

int console_print(const char *fmt, ...);
int console_println(const char *fmt, ...);
int console_newline();

#endif
