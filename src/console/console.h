#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "../drivers/stdio/emb-stdio.h"
#include "../hal/hal.h"
#include "../coreutils/coreutils.h"
#include "command.h"

void console_init();
void console_run();

void getcwd(char buf[]);
void chdir(const char *dir);

int console_print(const char *fmt, ...);
int console_println(const char *fmt, ...);
int console_newline();

#endif
