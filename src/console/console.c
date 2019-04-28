#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"
#include "../hal/hal.h"

int vconsole_print(const char *fmt, va_list args);

int console_init()
{
}

int vconsole_print(const char *fmt, va_list args)
{
    char printf_buf[512];
    int printed = 0;

    printed = vsprintf(printf_buf, fmt, args);

    for (int i = 0; i < printed; i++)
    {
        hal_io_serial_putc(printf_buf[i]);
        hal_io_video_putc(printf_buf[i]);
    }

    return printed;
}

int console_print(const char *fmt, ...)
{
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = vconsole_print(fmt, args);
    va_end(args);

    return printed;
}

int console_println(const char *fmt, ...)
{
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = vconsole_print(fmt, args);
    va_end(args);

    return printed + console_print("%c", '\n');
}

#endif
