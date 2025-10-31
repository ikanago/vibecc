#include "vector.h"

#include <stdlib.h>

struct Vector *vector_new() {
    void **data = (void **)calloc(VECTOR_DEFAULT_CAPACITY, sizeof(void *));
    if (data == NULL) return NULL;

    struct Vector *v = malloc(sizeof(struct Vector));
    if (v == NULL) return NULL;
    v->data = data;
    v->size = 0;
    v->capacity = VECTOR_DEFAULT_CAPACITY;
    return v;
}

void vector_push(struct Vector *v, void *item) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        void **data = (void **)realloc(v->data, sizeof(void *) * v->capacity);
    }

    v->data[v->size++] = item;
}
