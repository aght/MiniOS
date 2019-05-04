#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool stricmp(const char *a, const char *b);
int strreplc(char *str, char oldc, char newc);

#endif
