#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

#include "../minilib/minilib.h"

int vsprintf(char *buf, const char *fmt, va_list arg);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);

#endif
