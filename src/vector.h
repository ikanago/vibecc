#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define VECTOR_DEFAULT_CAPACITY 16

struct Vector {
    void **data;
    size_t size;
    size_t capacity;
};

struct Vector *vector_new();
void vector_push(struct Vector *vector, void *item);

#endif  // VECTOR_H
