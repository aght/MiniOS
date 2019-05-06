#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    size_t size;
    size_t capacity;
    void** data;
} vector;

void vector_init(vector*);
void vector_destroy(vector*);
void vector_destroyf(vector*);

bool vector_add(vector*, void*);
bool vector_insert(vector*, size_t, void*);
bool vector_remove(vector*, size_t);
bool vector_removef(vector*, size_t);

void* vector_get(vector*, size_t);
bool vector_set(vector*, size_t, void*);

int vector_find(vector*, void*);
int vector_rfind(vector*, void*);
int vector_findc(vector*, void*, int (*)(const void*, const void*));
int vector_rfindc(vector*, void*, int (*)(const void*, const void*));

bool vector_shrink(vector*);
bool vector_reserve(vector*, size_t);
bool vector_resize(vector*, size_t);

bool vector_contains(vector*, int (*)(const void*, const void*), void*);
void vector_reverse(vector*);

void vector_add_all(vector*, vector*);

void vector_clear(vector*);
void vector_clearf(vector*);
bool vector_is_empty(vector*);

void* vector_bsearch(vector*, int (*)(const void*, const void*), void*);
void vector_sort(vector*, int (*)(const void*, const void*));

#endif
