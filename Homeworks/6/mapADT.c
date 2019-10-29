#include "mapADT.h"

MapADT map_create(int (*compare)(const void *a, const void *b));
void map_destroy(MapADT map);
void map_clear(MapADT map);
bool map_contains(const MapADT map, const void *key);
void *map_put(MapADT map, void *key, void *value);
const void *map_get(const MapADT map, const void *key);
void *map_delete(MapADT map, const void *key);
bool map_empty(const MapADT map);
