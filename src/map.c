#include "map.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAP_DEFAULT_CAPACITY 16

struct Map *map_new_inner(size_t capacity) {
    struct Map *map = calloc(1, sizeof(struct Map));
    if (map == NULL) return NULL;

    char **keys = calloc(capacity, sizeof(char *));
    if (keys == NULL) return NULL;

    int **values = calloc(capacity, sizeof(int *));
    if (values == NULL) return NULL;

    map->keys = keys;
    map->values = values;
    map->size = 0;
    map->capacity = capacity;
    return map;
}

struct Map *map_new() {
    return map_new_inner(MAP_DEFAULT_CAPACITY);
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

int *map_get(struct Map *map, const char *key) {
    int i = find_slot(map, key);
    return map->values[i];
}

static void resize(struct Map *map) {
    struct Map *map2 = map_new_inner(map->capacity * 2);

    for (size_t i = 0; i < map->capacity; i++) {
        if (map->keys[i] == NULL) {
            continue;
        }
        map_set(map2, map->keys[i], map->values[i]);
        free(map->keys[i]);
    }

    free(map->keys);
    free(map->values);
    map->keys = map2->keys;
    map->values = map2->values;
    map->size = map2->size;
    map->capacity = map2->capacity;
    free(map2);
}

static int should_grow(struct Map *map) {
    return map->size > map->capacity * 6 / 10;
}

void map_set(struct Map *map, const char *key, int *value) {
    if (should_grow(map)) {
        resize(map);
    }

    int i = find_slot(map, key);
    int len = strlen(key);
    char *new_key = calloc(len + 1, sizeof(char));
    strncpy(new_key, key, len);
    map->keys[i] = new_key;

    map->values[i] = value;
    map->size++;
}
