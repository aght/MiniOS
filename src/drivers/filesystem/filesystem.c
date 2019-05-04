#ifndef _FILESYSTEM_
#define _FILESYSTEM_

#include "filesystem.h"

FILE fopen(const char *path) {
    return sdCreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
}

void fclose(FILE fh) {
    sdCloseHandle(fh);
}

bool fread(FILE fh, file_t *file) {
    if (fh) {
        file->file_size = sdGetFileSize(fh, NULL);

        file->bytes = (uint8_t*)malloc(sizeof(uint8_t) * (file->file_size + 1));

        u_int32_t bytes_read;

        sdReadFile(fh, file->bytes, file->file_size, &bytes_read, 0);

        return true;
    }

    return false;
}

#endif
