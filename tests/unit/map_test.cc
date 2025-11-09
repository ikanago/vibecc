#include "map.h"

#include <gtest/gtest.h>

TEST(MapTest, SetAndGetOnePair) {
    struct Map *map = map_new();
    int v = 42;
    map_set(map, "test", &v);
    ASSERT_EQ(v, *map_get(map, "test"));
}

TEST(MapTest, HashCollisionWhenKeyLengthIsOne) {
    struct Map *map = map_new();
    int v1 = 42;
    int v2 = 43;
    map_set(map, "@", &v1);  // @ => 0 = 64 % 16
    map_set(map, "P", &v2);  // P => 0 = 80 % 16
    ASSERT_EQ(v1, *map_get(map, "@"));
    ASSERT_EQ(v2, *map_get(map, "P"));
}

TEST(MapTest, HashCollisionWhenKeyLengthIsTwo) {
    struct Map *map = map_new();
    int v1 = 42;
    int v2 = 43;
    map_set(map, "@@", &v1);  // @ => 0 = 128 % 16
    map_set(map, "P@", &v2);  // P => 0 = 144 % 16
    ASSERT_EQ(v1, *map_get(map, "@@"));
    ASSERT_EQ(v2, *map_get(map, "P@"));
}

TEST(MapTest, ResizeOnce) {
    struct Map *map = map_new();
    int n_entries = map->capacity + 1;

    char buf[n_entries][7];
    int values[n_entries];
    for (int i = 0; i < n_entries; i++) {
        values[i] = i * 17 % 100;
        sprintf(buf[i], "test%d", values[i]);
        map_set(map, buf[i], values + i);
    }
    for (int i = 0; i < n_entries; i++) {
        int n = i * 17 % 100;
        int *value = map_get(map, buf[i]);
        ASSERT_EQ(n, *value);
    }
}
