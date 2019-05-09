#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <stdbool.h>

#include "../sdcard/SDCard.h"

typedef struct {
    uint8_t *bytes;
    uint32_t file_size;
} file_t;

typedef HANDLE FILE;

FILE fopen(const char *path);
void fclose(FILE fh);
bool fread(FILE fh, file_t *file);

#endif
