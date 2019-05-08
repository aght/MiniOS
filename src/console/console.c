#ifndef _CONSOLE_
#define _CONSOLE_

#include "console.h"

#define SCREEN_MAX_LINES SCREEN_HEIGHT / FONT_HEIGHT

#define USERNAME "andy"
#define HOSTNAME "localhost"

static int vconsole_print(const char *, va_list);
static int svconsole_print(const char *fmt, va_list args);

static int sconsole_print(const char *fmt, ...);
static int sconsole_println(const char *fmt, ...);
static int sconsole_newline();

static int execute_input(vector *buffer);
static int run_program(char *tokens[], int n);
static int run_command(char *tokens[], int n);

static void redraw();
static void prompt();
static int parse_input(vector *input, char *buffer[]);
static int parse_color_escape(const char *str);

typedef struct {
    uint8_t ascii_color;
} color_state_t;

static color_state_t state;

// For history
static vector printed_lines;
static vector printed_buffer;

// For user input
static vector input_buffer;

static char cwd[512];

void console_init() {
    state.ascii_color = 255;

    vector_init(&input_buffer);

    vector_init(&printed_lines);
    vector_init(&printed_buffer);

    hal_io_video_set_brush_color(ascii_colors[state.ascii_color - 16]);

    chdir(NULL);
    prompt();
}

void console_run() {
    while (1) {
        char input = hal_io_serial_getc();

        switch (input) {
            case '\n':
            case '\r': {
                console_newline();
                int status = execute_input(&input_buffer);

                if (printed_lines.size >= SCREEN_MAX_LINES && status != COMMAND_CLEAR) {
                    redraw();
                }

                if (status == COMMAND_CLEAR) {
                    vector_clearf(&printed_lines);
                }

                prompt();
                vector_clear(&input_buffer);
            } break;
            case '\b': {
                if (input_buffer.size > 0) {
                    console_print("\b \b");
                    vector_remove(&input_buffer, input_buffer.size - 1);
                }
            } break;
            default: {
                console_print("%c", input);
                vector_add(&input_buffer, input);
            }
        }
    }
}

static int execute_input(vector *buffer) {
    int n;
    int status;
    char *tokens[256];

    if (buffer->size == 0) {
        return COMMAND_FAILURE;
    }

    n = parse_input(buffer, tokens);

    status = run_command(tokens, n);

    if (status == COMMAND_NOT_FOUND) {
        console_println("%s: command not found", tokens[0]);
    }

    // run_program(tokens, n);
}

static int run_command(char *tokens[], int n) {
    command_t *cmd = find_command(tokens[0]);

    if (!cmd) {
        return COMMAND_NOT_FOUND;
    }

    return cmd->action(n > 1 ? &tokens[1] : NULL, n - 1);
}

static int run_program(char *tokens[], int n) {
    FILE fh;
    file_t file;
    char resolved_path[512];

    int type = realpath_n(tokens[0], resolved_path);

    switch (type) {
        case FILE_ATTRIBUTE_DIRECTORY:
            return COMMAND_FAILURE;
        case FILE_ATTRIBUTE_INVALID:
            return COMMAND_FAILURE;
    }

    fh = fopen(tokens[0]);

    if (fread(fh, &file)) {
        uint8_t bytes = file.bytes;

        int result = ((int (*)(void))(file.bytes))();

    } else {
        return COMMAND_FAILURE;
    }

    fclose(fh);
    free(file.bytes);
}

static int parse_input(vector *input, char *buffer[]) {
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

    return --j;
}

char *getcwd(char buf[]) {
    sprintf(buf, cwd);
    return cwd;
}

void chdir(const char *dir) {
    char concat_dir[512];
    char resolved_path[512];

    if (!dir) {
        sprintf(concat_dir, "/");
    } else if (strlen(dir) >= 1 && dir[0] == '/') {
        sprintf(concat_dir, "%s", dir);
    } else {
        sprintf(concat_dir, "%s/%s", cwd, dir);
    }

    int file_type = realpath_n(concat_dir, resolved_path);

    switch (file_type) {
        case FILE_ATTRIBUTE_DIRECTORY:
            sprintf(cwd, resolved_path);
            break;
        case FILE_ATTRIBUTE_NORMAL:
            console_println("%s: is a file", dir);
            break;
        default:
            console_println("%s: is not a file or directory", dir);
    }
}

void console_clear() {
    hal_io_video_clear();

    hal_io_set_cursor_x(0);
    hal_io_set_cursor_y(0);
}

static void redraw() {
    console_clear();

    // Add 1 to leave room for prompt
    int offset = printed_lines.size - SCREEN_MAX_LINES + 1;

    for (int i = offset - 1; i >= 0; i--) {
        vector_remove(&printed_lines, i);
    }

    for (int i = 0; i < printed_lines.size; i++) {
        char *line = vector_get(&printed_lines, i);
        i == printed_lines.size - 1 ? sconsole_println(line) : sconsole_println(line);
    }

    vector_clear(&printed_buffer);
}

static void prompt() {
    console_print("\\[034m%s@%s\\[255m:\\[027m%s\\[255m$ ", USERNAME, HOSTNAME, cwd);
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
        if (printf_buf[i] != '\n') {
            vector_add(&printed_buffer, printf_buf[i]);

            if (printed_lines.size > 2 * SCREEN_MAX_LINES) {
                redraw();
            }

        } else {
            int j = 0;
            char *printed_str;

            printed_str = (char *)malloc(sizeof(char) * printed_buffer.size + 1);

            for (j = 0; j < printed_buffer.size; j++) {
                printed_str[j] = vector_get(&printed_buffer, j);
            }
            printed_str[j] = '\0';

            vector_add(&printed_lines, printed_str);
            vector_clear(&printed_buffer);
        }
    }

    for (int i = 0; i < printed; i++) {
        if (printf_buf[i] == '\\') {
            int shift = parse_color_escape(&printf_buf[i]);

            hal_io_video_set_brush_color(ascii_colors[state.ascii_color - 16]);

            if (shift != 0) {
                i += shift;
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

static int svconsole_print(const char *fmt, va_list args) {
    char printf_buf[512];
    int printed = 0;

    printed = vsprintf(printf_buf, fmt, args);

    for (int i = 0; i < printed; i++) {
        if (printf_buf[i] == '\\') {
            int shift = parse_color_escape(&printf_buf[i]);

            hal_io_video_set_brush_color(ascii_colors[state.ascii_color - 16]);
            if (shift != 0) {
                i += shift;
            }
        }

        hal_io_video_putc(printf_buf[i]);
    }

    hal_io_video_set_brush_color(ascii_colors[255 - 16]);

    return printed;
}

static int sconsole_print(const char *fmt, ...) {
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = svconsole_print(fmt, args);
    va_end(args);

    return printed;
}

static int sconsole_println(const char *fmt, ...) {
    va_list args;
    int printed = 0;

    va_start(args, fmt);
    printed = svconsole_print(fmt, args);
    va_end(args);

    return printed + sconsole_print("%c", '\n');
}

static int sconsole_newline() {
    return sconsole_print("%c", '\n');
}

#endif
