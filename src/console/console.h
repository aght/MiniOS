#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdarg.h>

#include "../drivers/stdio/emb-stdio.h"

int console_init();
void console_run();
int console_print(const char *fmt, ...);
int console_println(const char *fmt, ...);
int console_newline();

#endif
