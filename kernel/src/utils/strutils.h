#ifndef _STRUTILS_H_
#define _STRUTILS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../vector/vector.h"

bool stricmp(const char *a, const char *b);
int strreplc(char *str, char oldc, char newc);
char *vtostr(vector *vec, char buffer[]);
int vstrrcc(vector *str, char c);

#endif
