#ifndef _COMMAND_
#define _COMMAND_

#include "command.h"

static bool pwd(const char *params[], int n);
static bool cd(const char *params[], int n);
static bool ls(const char *params[], int n);
static bool cat(const char *params[], int n);

static command_t command_list[] = {
    {"pwd", pwd},
    {"cd", cd},
    {"ls", ls},
    {"cat", cat}};

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

    console_println(path);

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
}

#endif
