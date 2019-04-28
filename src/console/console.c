#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"

#define USERNAME "andy"
#define HOSTNAME "localhost"

static int vconsole_print(const char *fmt, va_list args);
static void prompt();

static rgb_t text_color = {255, 255, 255};
static rgb_t fill_color = {0, 0, 0};

static char cwd[4096];

void console_init()
{
    hal_io_video_set_brush_color(text_color);
    hal_io_video_set_fill_color(fill_color);
    cd("/");
    prompt();
}

void console_run()
{
    while (1)
    {
        char input = hal_io_serial_getc();

        switch (input)
        {
        case '\n':
        case '\r':
            console_newline();
            prompt();
            break;
        case '\b':
            console_print("\b \b");
            break;
        default:
        {
            console_print("%c", input);
        }
        }
    }
}

void cd(const char* dir) {
    sprintf(cwd, dir);
}

static void prompt() {
    console_print("%s@%s:%s$ ", USERNAME, HOSTNAME, cwd);
}

static int vconsole_print(const char *fmt, va_list args)
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

int console_newline()
{
    return console_print("%c", '\n');
}

#endif
