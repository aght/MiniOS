#ifndef _COREUTILS_H_
#define _COREUTILS_H_

#include <string.h>

#include "../utils/strutils.h"
#include "../vector/vector.h"
#include "../console/console.h"

int realpath_n(const char *path, char *resolved_path);
int trpath(const char* path, const char* cwd, const char* append, char buffer[]);

#endif
