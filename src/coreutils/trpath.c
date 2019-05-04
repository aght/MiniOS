#ifndef _TRPATH_
#define _TRPATH_

#include "coreutils.h"

int trpath(const char *path, const char *cwd, const char *append, char buffer[]) {
    if (!buffer) {
        return FILE_ATTRIBUTE_INVALID;
    }

    char translated_path[512];

    if (strcmp(path, "/") == 0) {
        cwd = NULL;
    }

    if (path && cwd) {
        sprintf(translated_path, "%s/%s", cwd, path);
    } else if (path && !cwd) {
        sprintf(translated_path, "%s", path);
    } else if (!path && cwd) {
        sprintf(translated_path, "%s", cwd);
    }

    int file_type = realpath_n(translated_path, translated_path);

    strreplc(translated_path, '/', '\\');

    if (append) {
        if (strcmp(translated_path, "\\") == 0) {
            sprintf(translated_path, "%s%s", translated_path, append);
        } else {
            sprintf(translated_path, "%s\\%s", translated_path, append);
        }
    }

    strcpy(buffer, translated_path);

    return file_type;
}

#endif
