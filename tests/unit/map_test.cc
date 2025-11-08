#include "map.h"

#include <gtest/gtest.h>

TEST(MapTest, SetAndGetOnePair) {
    struct Map *map = map_new();
    map_set(map, "test", 42);
    int value = map_get(map, "test");
    ASSERT_EQ(42, value);
}
