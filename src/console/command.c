#ifndef _COMMAND_
#define _COMMAND_

#include "command.h"

static bool pwd(const char *params[], int n);
static bool cd(const char *params[], int n);
static bool ls(const char *params[], int n);

static command_t command_list[] = {
    {"pwd", pwd},
    {"cd", cd},
    {"ls", ls}};

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
            break;
        case FILE_ATTRIBUTE_INVALID:
            break;
    }

    HANDLE fh;
    FIND_DATA find;
    fh = sdFindFirstFile(path, &find);
    do {
        int accessHour12 = find.CreateDT.tm_hour > 12 ? find.CreateDT.tm_hour - 12 : find.CreateDT.tm_hour == 0 ? 12 : find.CreateDT.tm_hour;

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

#endif
