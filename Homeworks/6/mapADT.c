#include "mapADT.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* struct typdef for the key-value pair to be stored in the map */
typedef struct entry_s {
    void *key;
    void *value;
    struct entry_s *next;
} entry_t;

/* the mapStruct to conform to the header file */
struct mapStruct {
    entry_t *map;
    int (*compare)(const void *a, const void *b);
};

/* returns a map, where the compare function pointer is stored in the struct */
MapADT map_create(int (*compare)(const void *a, const void *b)) {
    MapADT map = (MapADT)calloc(1, sizeof(struct mapStruct));
    if (NULL != map)
        map->compare = compare;
    return map;
}
/* places an element [key, value] pair into the map. If the key already
 * exists, then update the entry with the new value and return the old value */
void *map_put(MapADT map, void *key, void *value) {
    entry_t *newNode;
    void *returnVal;
    int compare = 0;
    /* check if the map is empty */
    if (!map_empty(map)) {
        /* if the map already has the value. It will be replaced */
        entry_t *currNode = map->map; /* start iterating at beginning */
        entry_t *prevNode = NULL;
        /* iterate through, placing the keys alphabetically */
        while (currNode && (compare = map->compare(currNode->key, key)) < 0) {
            prevNode = currNode; /* traverse through */
            currNode = currNode->next;
        }
        /* compare stores the last comparison result, if it is zero, then the
         * item is already in the map and needs to be updated*/
        if (!compare) {
            if (!currNode) { /* if adding to the end of the map */
                currNode = prevNode;
            }
            returnVal = currNode->value; /* preserve the old value */
            currNode->value = value;     /* update the value */
        } else {                         /* if != 0, then it has to be added */
            newNode = (entry_t *)calloc(1, sizeof(entry_t));
            newNode->next = currNode; /* adding newnode before current node */
            newNode->key = key;       /* set the key */
            newNode->value = value;   /* set the value */
            if (prevNode) /* prevNode is only null if inserting at start */
                prevNode->next = newNode;
            else /* if inserting at beginning, hten update head of map */
                map->map = newNode;
            returnVal = NULL;
        }
    } else { /* the item being added is the first element */
        newNode = (entry_t *)calloc(1, sizeof(entry_t));
        map->map = newNode;   /* set the head to the new node */
        newNode->next = NULL; /* there isn't anything after this */
        newNode->key = key;   /* set the key and value */
        newNode->value = value;
        returnVal = NULL; /* didn't update anything */
    }
    return returnVal;
}
void map_clear(MapADT map) {
    entry_t *currNode = map->map, *next = NULL;
    /* traverse through the map and free all of the things */
    while (currNode) {
        next = currNode->next;
        free(currNode->value);
        free(currNode->key);
        free(currNode);
        currNode = next;
    }
    map->map = NULL; /* set the map to NULL, so not dangling */
}
bool map_contains(const MapADT map, const void *key) {
    entry_t *currNode = map->map;
    /* traverse through looking for item */
    while (currNode && map->compare(currNode->key, key)) {
        currNode = currNode->next;
    }
    /* if currNode is NULL at end, that means nothing was found */
    if (NULL == currNode) {
        return false;
    } else {
        return true;
    }
}
const void *map_get(const MapADT map, const void *key) {
    assert(!map_empty(map)); /* abort program if map empty */
    entry_t *currNode = map->map;
    /* traverse through map looking for key */
    while (currNode && map->compare(currNode->key, key)) {
        currNode = currNode->next;
    }
    /* if currNode == NULL, then it wasn't in the map */
    if (NULL == currNode) {
        return NULL;
    } else {
        return currNode->value;
    }
}
void *map_delete(MapADT map, const void *key) {
    assert(!map_empty(map)); /* abort program if map is empty */
    entry_t *currNode = map->map;
    entry_t *prev = NULL;
    void *temp = NULL;
    /* traverse through map */
    while (currNode && map->compare(currNode->key, key)) {
        prev = currNode;
        currNode = currNode->next;
    }
    /* if NULL, then it wasn't found. Meaning it can't be deleted */
    if (NULL == currNode)
        return NULL;
    temp = currNode->value;
    /* if there is a previous node, meaning it isn't the first thing in the
     * map, then set the previous's next to point to the current next.
     * Effectively excising the current node from map */
    if (NULL != prev)
        prev->next = currNode->next;
    /* else that means it is the first item being deleted, so set the front of
     * the map to the thing */
    else
        map->map = currNode->next;
    /* free the things */
    free(currNode->key);
    free(currNode);
    return temp; /* return the value removed. This assumes it will be freed
                    after */
}
bool map_empty(const MapADT map) {
    /* returns if the map linked list is empty */
    return map->map == NULL;
}
void map_destroy(MapADT map) {
    map_clear(map); /* empty out the map */
    free(map);      /* free the space allocated for the map */
}
