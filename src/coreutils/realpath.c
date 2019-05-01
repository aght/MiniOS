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

static void tokenize(vector *v, char delim, vector *tokens) {
    mark_delims(v, delim);

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

        if (!(k - j == 1 && vector_get(v, j + 1) == '\0')) {
            vector *str = (vector *)malloc(sizeof(vector));
            vector_init(str);

            for (int z = j + 1; z < k; z++) {
                vector_add(str, vector_get(v, z));
            }

            vector_add(tokens, str);
        }

        j = i = k;
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

static void resolve_symbols(vector *tokens) {
    for (int i = 0; i < tokens->size; i++) {
        vector *str = vector_get(tokens, i);
        int c = vstrrcc(str, '.');

        if ((c == 2 && str->size > 2) || (c == 1 && str->size > 1)) {
            vector *a = (vector *)malloc(sizeof(vector));
            vector *b = vector_get(tokens, i);

            vector_init(a);

            for (int k = 0; k < c; k++) {
                vector_add(a, '.');
                vector_remove(b, vector_rfind(b, '.'));
            }

            vector_insert(tokens, i + 1, a);

            i++;
        }
    }
}

static bool m_strcmp(vector *a, const char *b) {
    if (!a || !b || a->size != strlen(b)) {
        return false;
    }

    for (int i = 0; i < a->size; i++) {
        if ((char)vector_get(a, i) != b[i]) {
            return false;
        }
    }

    return true;
}

static void build_path(vector *tokens, char *buffer) {
    vector build;
    vector_init(&build);

    for (int i = 0; i < tokens->size; i++) {
        vector *str = vector_get(tokens, i);

        if (m_strcmp(str, "..")) {
            if (build.size > 0) {
                vector_remove(&build, build.size - 1);
            }
            continue;
        } else if (m_strcmp(str, ".")) {
            continue;
        }

        vector_add(&build, vector_get(tokens, i));
    }

    int k = 0;

    for (int i = 0; i < build.size; i++) {
        vector *str = vector_get(&build, i);

        buffer[k++] = '/';

        for (int j = 0; j < str->size; j++) {
            buffer[k++] = vector_get(str, j);
        }

        console_newline();
    }

    buffer[k] = '\0';

    vector_destroy(&build);
}

char *realpath_n(const char *path, char *resolved_path) {
    vector m_str;
    vector tokens;

    vector_init(&m_str);
    vector_init(&tokens);

    for (int i = 0; i < strlen(path) + 1; i++) {
        vector_add(&m_str, path[i]);
    }

    tokenize(&m_str, '/', &tokens);
    resolve_symbols(&tokens);
    build_path(&tokens, resolved_path);

    vector_destroy(&m_str);
    vector_destroyf(&tokens);

    return resolved_path;
}

#endif
