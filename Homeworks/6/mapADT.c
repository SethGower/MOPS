#include "mapADT.h"
#include <assert.h>
#include <stdlib.h>

typedef struct entry_s {
    void *key;
    void *value;
    struct entry_s *next;
} entry_t;
struct mapStruct {
    entry_t *map;
    int (*compare)(const void *a, const void *b);
};
MapADT map_create(int (*compare)(const void *a, const void *b)) {
    MapADT map = (MapADT)calloc(1, sizeof(struct mapStruct));
    if (NULL != map)
        map->compare = compare;
    return map;
}
void *map_put(MapADT map, void *key, void *value) {
    entry_t *newNode = (entry_t *)calloc(1, sizeof(entry_t));
    void *returnVal;
    int compare = 0;
    if (!map_empty(map)) {
        /* if the map already has the value. It will be replaced */
        entry_t *currNode = map->map;
        entry_t *prevNode = NULL;
        while (currNode && (compare = map->compare(currNode->key, key)) < 0) {
            prevNode = currNode;
            currNode = currNode->next;
        }
        if (map_contains(map, key)) {
            if (!currNode) {
                currNode = prevNode;
            }
            returnVal = currNode->value;
            currNode->value = value;
            free(newNode);
        } else {
            newNode->next = currNode;
            newNode->key = key;
            newNode->value = value;
            if (prevNode)
                prevNode->next = newNode;
            else
                map->map = newNode;
            returnVal = NULL;
        }
    } else {
        map->map = newNode;
        newNode->next = NULL;
        newNode->key = key;
        newNode->value = value;
        returnVal = NULL;
    }
    return returnVal;
}
void map_clear(MapADT map) {
    entry_t *currNode = map->map, *next = NULL;
    while (currNode) {
        next = currNode->next;
        free(currNode);
        currNode = next;
    }
    map->map = NULL;
}
bool map_contains(const MapADT map, const void *key) {
    entry_t *currNode = map->map;
    while (currNode && map->compare(currNode->key, key)) {
        currNode = currNode->next;
    }
    if (NULL == currNode) {
        return false;
    } else {
        return true;
    }
}
const void *map_get(const MapADT map, const void *key) {
    assert(!map_empty(map));
    entry_t *currNode = map->map;
    while (currNode && map->compare(currNode->key, key)) {
        currNode = currNode->next;
    }
    if (NULL == currNode) {
        return NULL;
    } else {
        return currNode->value;
    }
}
void *map_delete(MapADT map, const void *key) {
    assert(!map_empty(map));
    entry_t *currNode = map->map;
    entry_t *prev = NULL;
    void *temp = NULL;
    while (currNode && map->compare(currNode->key, key)) {
        prev = currNode;
        currNode = currNode->next;
    }
    if (NULL == currNode)
        return NULL;
    temp = currNode->value;
    if (NULL != prev)
        prev->next = currNode->next;
    free(currNode);
    return temp;
}
bool map_empty(const MapADT map) {
    /* returns if the map linked list is empty */
    return map->map == NULL;
}
void map_destroy(MapADT map) {
    map_clear(map);
    free(map);
}
