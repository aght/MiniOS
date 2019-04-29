#ifndef _COREUTILS_H_
#define _COREUTILS_H_

#include <string.h>

#include "../vector/vector.h"
#include "../console/console.h"

char *realpath_n(const char *path, const char *resolved_path);

#endif
