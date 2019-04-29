#ifndef _REALPATH_
#define _REALPATH_

#include "coreutils.h"

static void mark_delims(vector *v, char delim) {
    for (int i = 0; i < v->size; i++) {
        if (vector_get(v, i) == delim) {
            vector_set(v, i, '\0');
        }
    }
}

static void tokenize(vector *v, char token, vector tokens[], int *n) {
    mark_delims(v, token);

    *n = 0;

    int j = -1;
    int k = 0;

    for (int i = 0; i < v->size;) {
        if (vector_get(v, j + 1) == '\0') {
            j++;
        }

        for (int m = j + 1; m <= v->size; m++) {
            if (vector_get(v, m) == '\0') {
                k = m;
                break;
            }
        }

        if ((k == v->size || vector_get(v, j + 1) == '\0') && j >= k) {
            break;
        }

        if (k - j == 1 && vector_get(v, j + 1) == '\0') {
            j = k;
            i = k;
            continue;
        }

        vector_init(&tokens[*n]);

        console_println("%d, %d", j, k);
        for (int z = j + 1; z < k; z++) {
            vector_add(&tokens[*n], vector_get(v, z));
        }

        (*n)++;

        j = k;
        i = k;
    }
}

static int vstrrcc(vector *str, char c) {
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

static void resolve_symbols(vector tokens[], int n) {
    for (int i = 0; i < n; i++) {
        int c = vstrrcc(&tokens[i], '.');

        if (c == 2) {
        }

        for (int j = 0; j < tokens[i].size; j++) {
            console_print("%c", vector_get(&tokens[i], j));
        }
        console_println(": %d", vstrrcc(&tokens[i], '.'));
    }
}

char *realpath_n(const char *path, const char *resolved_path) {
    vector m_str;

    vector_init(&m_str);

    for (int i = 0; i < strlen(path) + 1; i++) {
        vector_add(&m_str, path[i]);
    }

    vector tokens[256];
    int n;

    tokenize(&m_str, '/', tokens, &n);
    resolve_symbols(tokens, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < tokens[i].size; j++) {
            console_print("%c", vector_get(&tokens[i], j));
        }
        console_newline();
    }

    vector_destroy(&m_str);

    return NULL;
}

#endif
