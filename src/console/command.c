#ifndef _COMMAND_
#define _COMMAND_

#include "command.h"

static bool pwd(const char *params[], int n);
static bool cd(const char *params[], int n);
static bool ls(const char *params[], int n);
static bool cat(const char *params[], int n);
static bool hexdump(const char *params[], int n);
static bool clear(const char* params[], int n);

static command_t command_list[] = {
    {"pwd", pwd},
    {"cd", cd},
    {"ls", ls},
    {"cat", cat},
    {"hexdump", hexdump},
    {"clear", clear}};

command_t *find_command(const char *str) {
    for (int i = 0; i < sizeof(command_list); i++) {
        if (strcmp(command_list[i].command, str) == 0) {
            return &command_list[i];
        }
    }

    return NULL;
}

static bool pwd(const char *params[], int n) {
    char cwd[512];
    getcwd(cwd);
    console_println(cwd);

    return true;
}

static bool cd(const char *params[], int n) {
    if (n == 0) {
        chdir(NULL);
    } else {
        chdir(params[0]);
    }

    return true;
}

static bool ls(const char *params[], int n) {
    char cwd[512];
    char path[512];
    int file_type;

    file_type = trpath(n != 0 ? params[0] : NULL, getcwd(cwd), "*.*", path);

    switch (file_type) {
        case FILE_ATTRIBUTE_NORMAL:
            console_println("%s: is a file", params[0]);
            return false;
        case FILE_ATTRIBUTE_INVALID:
            console_println("%s: is not a file or directory", params[0]);
            return false;
    }

    HANDLE fh;
    FIND_DATA find;
    fh = sdFindFirstFile(path, &find);
    do {
        int accessHour12 = (find.CreateDT.tm_hour - 1) % 12 + 1;

        console_println("%02d/%02d/%4d  %02d:%02d %2s  %5s  %9lu  %s",
                        find.CreateDT.tm_mon,
                        find.CreateDT.tm_mday,
                        find.CreateDT.tm_year + 1900,
                        accessHour12,
                        find.CreateDT.tm_min,
                        find.CreateDT.tm_hour < 12 ? "AM" : "PM",
                        find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ? "<DIR>" : "",
                        (unsigned long)find.nFileSizeLow,
                        find.cFileName);
    } while (sdFindNextFile(fh, &find) != 0);
    sdFindClose(fh);

    return true;
}

static bool cat(const char *params[], int n) {
    char cwd[512];
    char path[512];
    int file_type;

    file_type = trpath(n != 0 ? params[0] : NULL, getcwd(cwd), NULL, path);

    switch (file_type) {
        case FILE_ATTRIBUTE_DIRECTORY:
            console_println("%s: is a directory", path);
            return false;
        case FILE_ATTRIBUTE_INVALID:
            console_println("%s: is not a file or directory", path);
            return false;
    }

    FILE fh;
    file_t file;

    fh = fopen(path);

    if (fread(fh, &file)) {
        for (int i = 0; i < file.file_size; i++) {
            console_print("%c", (char)file.bytes[i]);
        }
        console_newline();
    } else {
        console_println("unable to read file");
    }

    fclose(fh);
    free(file.bytes);

    return true;
}

static bool hexdump(const char *params[], int n) {
    char cwd[512];
    char path[512];
    int file_type;

    file_type = trpath(n != 0 ? params[0] : NULL, getcwd(cwd), NULL, path);

    switch (file_type) {
        case FILE_ATTRIBUTE_DIRECTORY:
            console_println("%s: is a directory", path);
            return false;
        case FILE_ATTRIBUTE_INVALID:
            console_println("%s: is not a file or directory", path);
            return false;
    }

    FILE fh;
    file_t file;

    fh = fopen(path);

    if (fread(fh, &file)) {
        const int bytes_per_line = 16;
        const int bytes_per_section = 2;

        for (int i = 0; i < file.file_size; i += bytes_per_line) {
            console_print("%08x  ", i);

            for (int j = 0, n = i; j < bytes_per_line / bytes_per_section; j++) {
                for (int k = 0; k < bytes_per_section; k++, n++) {
                    if (n < file.file_size) {
                        console_print("%02x", file.bytes[n]);
                    } else {
                        console_print("%2s", "");
                    }
                }
                console_print(" ");
            }

            console_print(" ");

            for (int j = 0, k = i; j < bytes_per_line; j++, k++) {
                if (k < file.file_size) {
                    if ((char)file.bytes[k] == '\n' || (char)file.bytes[k] == '\r') {
                        console_print("%c", '.');
                    } else {
                        console_print("%c", (char)file.bytes[k]);
                    }
                }
            }

            console_newline();
        }
    } else {
        console_println("unable to read file");
    }

    fclose(fh);
    free(file.bytes);

    return true;
}

static bool clear(const char* params[], int n) {
    for (uint_fast32_t i = 0; i < SCREEN_HEIGHT; i++) {
        for (uint_fast32_t j = 0; j < SCREEN_WIDTH; j++) {
            hal_io_video_put_pixel(j, i, ascii_colors[16 - 16]);
        }
    }

    hal_io_set_cursor_x(0);
    hal_io_set_cursor_y(0); 
}

#endif
