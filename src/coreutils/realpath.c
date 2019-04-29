#ifndef _REALPATH_
#define _REALPATH_

#include "coreutils.h"

static void mark_tokens(vector *v, char token) {
    for (int i = 0; i < v->size; i++) {
        if (vector_get(v, i) == token) {
            vector_set(v, i, '\0');
        }
    }
}

static void tokenize(vector *v, char token, vector tokens[], int *n) {
    mark_tokens(v, token);

    *n = 0;

    int j = -1;
    int k = 0;

    for (int i = 0; i < v->size;) {
        for (int m = j + 1; m <= v->size; m++) {
            if (vector_get(v, m) == '\0') {
                k = m;
                break;
            }
        }

        if (k == v->size || vector_get(v, j + 1) == '\0') {
            break;
        }

        vector_init(&tokens[*n]);
        
        for (int z = j + 1; z < k; z++) {
            vector_add(&tokens[*n], vector_get(v, z));
        }

        (*n)++;

        j = k;
        i = k;
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
