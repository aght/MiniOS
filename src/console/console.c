#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"

#define USERNAME "andy"
#define HOSTNAME "localhost"

static int vconsole_print(const char *, va_list);
static void prompt();
static bool run_command(const char *);

static rgb_t text_color = {255, 255, 255};
static rgb_t fill_color = {0, 0, 0};

static char input_buffer[SCREEN_WIDTH / FONT_WIDTH];
static char cwd[4096];

void console_init()
{
    hal_io_video_set_brush_color(text_color);
    hal_io_video_set_fill_color(fill_color);
    change_directory("/");
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
            if (run_command(input_buffer))
            {
                console_newline();
            }
            else
            {
                console_println("%s: command not found", input_buffer);
            }

            prompt();
            memset(input_buffer, 0, sizeof(input_buffer));
            break;
        case '\b':
            console_print("\b \b");
            break;
        default:
        {
            console_print("%c", input);
            sprintf(input_buffer, "%s%c", input_buffer, input);
        }
        }
    }
}

const char *get_cwd()
{
    return cwd;
}

void change_directory(const char *dir)
{
    sprintf(cwd, dir);
}

static void prompt()
{
    console_print("%s@%s:%s$ ", USERNAME, HOSTNAME, cwd);
}

static bool run_command(const char *buf)
{
    command_t *cmd = find_command(buf);

    if (!cmd)
    {
        return false;
    }

    cmd->action("0");

    return true;
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
