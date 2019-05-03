#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"

#define USERNAME "andy"
#define HOSTNAME "localhost"

static int vconsole_print(const char *, va_list);
static void prompt();
static void run_command(vector *buffer);
static void input_parse(vector *input, char *buffer[], int *n);
static int parse_color_escape(const char *str);

typedef struct {
    uint8_t ascii_color;
} color_state_t;

static color_state_t state;

static rgb_t text_color = {255, 255, 255};
static rgb_t fill_color = {0, 0, 0};

static vector input_buffer;
static char cwd[512];

void console_init() {
    state.ascii_color = 255;

    vector_init(&input_buffer);
    hal_io_video_set_brush_color(ascii_colors[state.ascii_color - 16]);
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
                if (input_buffer.size > 0) {
                    console_print("\b \b");
                    vector_remove(&input_buffer, input_buffer.size - 1);
                }
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
        sprintf(cwd, "/");
        return;
    }

    char concat_dir[512];
    sprintf(concat_dir, "%s/%s", cwd, dir);

    char resolved_path[512];
    realpath_n(concat_dir, resolved_path);

    sprintf(cwd, resolved_path);
}

static void prompt() {
    console_print("\\[034m%s@%s\\[255m:\\[027m%s\\[255m$ ", USERNAME, HOSTNAME, cwd);
}

static void run_command(vector *buffer) {
    if (buffer->size == 0) {
        return;
    }

    char *tokens[256];
    int n;

    input_parse(buffer, tokens, &n);

    command_t *cmd = find_command(tokens[0]);

    if (!cmd) {
        console_println("%s: command not found", tokens[0]);
        return;
    }

    cmd->action(n > 1 ? &tokens[1] : NULL, n - 1);
}

static void input_parse(vector *input, char *buffer[], int *n) {
    char str[input->size + 1];

    int i;
    for (i = 0; i < input->size; i++) {
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

static int parse_color_escape(const char *str) {
    int len = strlen(str);

    if (len < 4 || str[1] != '[') {
        return 0;
    }

    char *ptr = &str[2];
    int n = 0;

    while (isdigit(*ptr)) {
        ptr++;
        n++;
    }

    int val = strtol(&str[2], NULL, 10);

    if (2 + n >= len || str[2 + n] != 'm') {
        return 0;
    }

    if (val < 16 || val > 255 || n > 3 || n < 1) {
        state.ascii_color = 255;
    } else {
        state.ascii_color = val;
    }

    return 2 + n + 1;
}

static int vconsole_print(const char *fmt, va_list args) {
    char printf_buf[512];
    int printed = 0;

    printed = vsprintf(printf_buf, fmt, args);

    for (int i = 0; i < printed; i++) {

        if (printf_buf[i] == '\\') {
            int status = parse_color_escape(&printf_buf[i]);

            hal_io_video_set_brush_color(ascii_colors[state.ascii_color - 16]);
            if (status != 0) {
                i += status;
            }
        }

        hal_io_serial_putc(printf_buf[i]);
        hal_io_video_putc(printf_buf[i]);
    }

    hal_io_video_set_brush_color(ascii_colors[255 - 16]);

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
