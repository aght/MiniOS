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
    

#endif
