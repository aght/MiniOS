#ifndef _STRUTILS_
#define _STRUTILS_

#include "strutils.h"

bool stricmp(const char *a, const char *b) {
    if (!a || !b || strlen(a) != strlen(b)) {
        return false;
    }

    for (int i = 0; a[i] != '\0'; i++) {
        if (tolower(a[i]) != tolower(b[i])) {
            return false;
        }
    }

    return true;
}

int strreplc(char *str, char oldc, char newc) {
    if (!str) {
        return 0;
    }

    int n = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == oldc) {
            str[i] = newc;
            n++;
        }
    }

    return n;
}

char *vtostr(vector *vec, char buffer[]) {
    if (!vec) {
        return NULL;
    }

    int j;
    for (j = 0; j < vec->size; j++) {
        buffer[j] = vector_get(vec, j);
    }
    buffer[j] = '\0';

    return buffer;
}

int vstrrcc(vector *str, char c) {
    int n = 0;

    for (int i = str->size - 1; i >= 0; i--) {
        if (vector_get(str, i) == c) {
            n++;
        } else {
            return n;
        }
    }

    return n;
}

#endif
