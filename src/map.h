#ifndef MAP_H
#define MAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Map {
    char **keys;
    int *values;
    size_t capacity;
};

struct Map *map_new();
int map_get(struct Map *map, const char *key);
void map_set(struct Map *map, const char *key, const int value);

#ifdef __cplusplus
}
#endif

#endif
