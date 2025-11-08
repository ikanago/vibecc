#include "map.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAP_DEFAULT_CAPACITY 16

struct Map *map_new() {
    struct Map *map = calloc(1, sizeof(struct Map));
    if (map == NULL) return NULL;

    char **keys = calloc(MAP_DEFAULT_CAPACITY, sizeof(char *));
    if (keys == NULL) return NULL;

    int *values = calloc(MAP_DEFAULT_CAPACITY, sizeof(int));
    if (values == NULL) return NULL;

    map->keys = keys;
    map->values = values;
    map->capacity = MAP_DEFAULT_CAPACITY;
    return map;
}

static int hash(const char *key, int mod) {
    int hashed = 0;
    for (const char *c = key; *c != '\0'; c++) {
        hashed += (int)*c;
        hashed %= mod;
    }
    return hashed;
}

static int find_slot(struct Map *map, const char *key) {
    int i = hash(key, map->capacity);
    while (map->keys[i] != NULL && strcmp(key, map->keys[i])) {
        i++;
        i %= map->capacity;
    }
    return i;
}

int map_get(struct Map *map, const char *key) {
    int i = find_slot(map, key);
    return map->values[i];
}

void map_set(struct Map *map, const char *key, const int value) {
    int i = find_slot(map, key);

    int len = strlen(key);
    char *new_key = calloc(len + 1, sizeof(char));
    strncpy(new_key, key, len);
    map->keys[i] = new_key;
    map->values[i] = value;
}
