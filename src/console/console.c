#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"

#define USERNAME "andy"
#define HOSTNAME "localhost"

static int vconsole_print(const char *, va_list);
static void prompt();
static void run_command(vector *buffer);
static void input_parse(vector *input, char *buffer[], int *n);

static rgb_t text_color = {255, 255, 255};
static rgb_t fill_color = {0, 0, 0};

static vector input_buffer;
static char cwd[4096];

void console_init() {
    vector_init(&input_buffer);
    hal_io_video_set_brush_color(text_color);
    hal_io_video_set_fill_color(fill_color);
    chdir(NULL);
    prompt();
}

void console_run() {
    while (1) {
        char input = hal_io_serial_getc();

        switch (input) {
            case '\n':
            case '\r':
                console_newline();
                run_command(&input_buffer);
                prompt();
                vector_clear(&input_buffer);
                break;
            case '\b':
                console_print("\b \b");
                vector_remove(&input_buffer, input_buffer.size - 1);
                break;
            default: {
                console_print("%c", input);
                vector_add(&input_buffer, input);
            }
        }
    }
}

void getcwd(char buf[]) {
    sprintf(buf, cwd);
}

void chdir(const char *dir) {
    if (!dir) {
        strcpy(cwd, "/");
        return;
    }

    char resolved_path[4096];
    realpath_n(dir, resolved_path);
}

static void prompt() {
    console_print("%s@%s:%s$ ", USERNAME, HOSTNAME, cwd);
}

static void run_command(vector *buffer) {
    char *tokens[256];
    int n;

    input_parse(buffer, tokens, &n);

    command_t *cmd = find_command(tokens[0]);

    if (!cmd) {
        console_println("%s: command not found", tokens[0]);
        return;
    }

    if (n > 1) {
        cmd->action(&tokens[1], n - 1);
    } else {
        cmd->action(NULL, n - 1);
    }
}

static void input_parse(vector *input, char *buffer[], int *n) {
    char str[input->size + 1];

    int i;
    for (i = 0; i < input->size + 1; i++) {
        str[i] = vector_get(input, i);
    }
    str[i] = '\0';

    int j = 0;

    char *token = strtok(str, " ");
    buffer[j++] = token;
    while (token) {
        token = strtok(NULL, " ");
        buffer[j++] = token;
    }

    *n = --j;
}

static int vconsole_print(const char *fmt, va_list args) {
    char printf_buf[512];
    int printed = 0;

    printed = vsprintf(printf_buf, fmt, args);

    for (int i = 0; i < printed; i++) {
        hal_io_serial_putc(printf_buf[i]);
        hal_io_video_putc(printf_buf[i]);
    }

    return printed;
}

int console_print(const char *fmt, ...) {
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = vconsole_print(fmt, args);
    va_end(args);

    return printed;
}

int console_println(const char *fmt, ...) {
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = vconsole_print(fmt, args);
    va_end(args);

    return printed + console_print("%c", '\n');
}

int console_newline() {
    return console_print("%c", '\n');
}

#endif
