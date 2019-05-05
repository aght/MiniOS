#ifndef _VECTOR_
#define _VECTOR_

#include "vector.h"

#define ALLOC_BLOCK_SIZE 10

bool index_in_range(vector *, size_t);
void swap(void **a, void **b);

void insertion_sort(vector *v, int (*cmp)(const void *, const void *));

void vector_init(vector *v) {
    v->size = 0;
    v->capacity = 0;
    v->data = NULL;
}

void vector_destroy(vector *v) {
    free(v->data);

    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void vector_destroyf(vector *v) {
    for (int i = 0; i < v->size; i++) {
        free(v->data[i]);
    }

    vector_destroy(v);
}

bool vector_add(vector *v, void *e) {
    if (v->size == 0) {
        v->data = malloc(ALLOC_BLOCK_SIZE * sizeof(void *));

        if (!v->data) {
            return false;
        }

        v->capacity = ALLOC_BLOCK_SIZE;
    }

    if (v->size == v->capacity) {
        void **tmp = realloc(v->data, (v->size + ALLOC_BLOCK_SIZE) * sizeof(void *));

        if (!tmp) {
            return false;
        }

        v->data = tmp;
        v->capacity += ALLOC_BLOCK_SIZE;
    }

    v->data[v->size] = e;
    v->size++;

    return true;
}

bool vector_insert(vector *v, size_t index, void *e) {
    if (!index_in_range(v, index)) {
        return false;
    }

    vector tmp;
    vector_init(&tmp);

    for (int i = index; i < v->size; i++) {
        vector_add(&tmp, vector_get(v, i));
    }

    vector_set(v, index, e);
    
    for (int i = 0, k = index + 1; i < tmp.size; i++, k++) {
        if (k < v->size) {
            vector_set(v, k, vector_get(&tmp, i));
        } else {
            vector_add(v, vector_get(&tmp, i));
        }
    }

    vector_destroy(&tmp);

    return true;
}

bool vector_remove(vector *v, size_t index) {
    if (!index_in_range(v, index)) {
        return false;
    }

    memcpy(&v->data[index], &v->data[index + 1], (v->capacity - index) * sizeof(void *));

    v->size--;

    return true;
}

bool vector_removef(vector* v, size_t index) {
    int success = vector_remove(v, index);
    
    if (success) {
        free(v->data[index]);
    }

    return success;
}

void *vector_get(vector *v, size_t index) {
    if (!index_in_range(v, index)) {
        return NULL;
    }

    return v->data[index];
}

bool vector_set(vector *v, size_t index, void *e) {
    if (!index_in_range(v, index)) {
        return false;
    }

    v->data[index] = e;

    return true;
}

int vector_find(vector *v, void *e) {
    for (int i = 0; i < v->size; i++) {
        if (v->data[i] == e) {
            return i;
        }
    }

    return -1;
}

int vector_rfind(vector *v, void *e) {
    for (int i = v->size - 1; i >= 0; i--) {
        if (v->data[i] == e) {
            return i;
        }
    }

    return -1;
}

int vector_findc(vector *v, void *e, int (*cmp)(const void *, const void *)) {
    for (int i = 0; i < v->size; i++) {
        if (cmp(&v->data[i], &e) == 0) {
            return i;
        }
    }

    return -1;
}

int vector_rfindc(vector *v, void *e, int (*cmp)(const void *, const void *)) {
    for (int i = v->size - 1; i >= 0; i--) {
        if (cmp(&v->data[i], &e) == 0) {
            return i;
        }
    }

    return -1;
}

bool vector_shrink(vector *v) {
    return vector_resize(v, v->size);
}

bool vector_reserve(vector *v, size_t n) {
    if (n < v->capacity) {
        return false;
    }

    bool result = vector_resize(v, n);

    return result;
}

bool vector_resize(vector *v, size_t n) {
    if (v->size == 0) {
        v->data = malloc(n * sizeof(void *));
        if (!v->data) {
            return false;
        }
    } else {
        void **tmp = realloc(v->data, n * sizeof(void *));

        if (!tmp) {
            return false;
        }

        v->data = tmp;
    }

    v->capacity = n;

    if (n < v->size) {
        v->size = n;
    }

    return true;
}

bool vector_contains(vector *v, int (*cmp)(const void *, const void *), void *e) {
    for (size_t i = 0; i < v->size; i++) {
        if (cmp(&e, &v->data[i]) == 0) {
            return true;
        }
    }

    return false;
}

void vector_reverse(vector *v) {
    for (int i = 0, j = v->size - 1; i < v->size / 2; i++, j--) {
        swap(&v->data[i], &v->data[j]);
    }
}

void vector_add_all(vector *dest, vector *src) {
    for (int i = 0; i < src->size; i++) {
        vector_add(dest, src->data[i]);
    }
}

void vector_clear(vector *v) {
    vector_destroy(v);
    vector_init(v);
}

void vector_clearf(vector *v) {
    vector_destroyf(v);
    vector_init(v);
}

bool vector_is_empty(vector *v) {
    return v->size == 0;
}

void *vector_bsearch(vector *v, int (*cmp)(const void *, const void *), void *e) {
    int l = 0;
    int h = v->size - 1;

    while (l <= h) {
        int mid = l + (h - l) / 2;

        if (cmp(&e, &v->data[mid]) == 0) {
            return &v->data[mid];
        } else if (cmp(&e, &v->data[mid]) < 0) {
            h = mid - 1;
        } else {
            l = mid + 1;
        }
    }

    return NULL;
}

void insertion_sort(vector *v, int (*cmp)(const void *, const void *)) {
    long long i, j;
    void *p;

    for (i = 1; i < v->size; i++) {
        p = v->data[i];
        j = i - 1;

        while (j >= 0 && cmp(&p, &v->data[j]) < 0) {
            v->data[j + 1] = v->data[j];
            j = j - 1;
        }

        v->data[j + 1] = p;
    }
}

void vector_sort(vector *v, int (*cmp)(const void *, const void *)) {
    qsort(v->data, v->size, sizeof(void *), cmp);
}

bool index_in_range(vector *v, size_t index) {
    return !(index >= v->size || index < 0);
}

void swap(void **a, void **b) {
    void *tmp = *b;
    *b = *a;
    *a = tmp;
}

#endif
