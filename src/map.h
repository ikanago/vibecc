#ifndef MAP_H
#define MAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Map {
    char **keys;
    int **values;
    int size;
    int capacity;
};

struct Map *map_new();
int *map_get(struct Map *map, const char *key);
void map_set(struct Map *map, const char *key, int *value);

#ifdef __cplusplus
}
#endif

#endif
