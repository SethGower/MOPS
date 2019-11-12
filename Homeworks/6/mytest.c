#define _DEFAULT_SOURCE

#include "mapADT.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
long int testData[] = {42, 17, -12, 9982, 476, 2912, -22, 3291213, 7782};
int numData = sizeof(testData) / sizeof(long int);

int compare_uint(const void *a, const void *b) {
    unsigned int na = *(unsigned int *)a;
    unsigned int nb = *(unsigned int *)b;

    return (na < nb ? -1 : (na == nb ? 0 : 1));
}
void process_uint(MapADT map) {
    int i;
    unsigned int *key;
    long int *value;
    unsigned int testKey;

    // load data into the map

    fputs("Inserting: ", stdout);
    for (i = 0; i < numData; ++i) {
        key = (unsigned int *)malloc(sizeof(int));
        assert(NULL != key);
        *key = i * 1000;
        value = (long int *)malloc(sizeof(long int));
        assert(NULL != value);
        *value = testData[i];
        printf("[%d, %ld] ", *key, *value);
        map_put(map, (void *)key, (void *)value);
    }

    // begin testing

    fputs("\nTesting contents:", stdout);
    testKey = 4000;
    printf("\n   Contains %u: %s", testKey,
           map_contains(map, (void *)&testKey) ? "true" : "false");
    testKey = 22000;
    printf("\n   Contains %u: %s", testKey,
           map_contains(map, (void *)&testKey) ? "true" : "false");
    testKey = 1000000;
    printf("\n   Contains %u: %s", testKey,
           map_contains(map, (void *)&testKey) ? "true" : "false");

    fputs("\nGetting:  ", stdout);
    for (i = numData - 1; i >= 0; --i) {
        testKey = i * 1000;
        value = (long int *)map_get(map, (void *)&testKey);
        assert(NULL != value);
        printf(" %ld", *value);
    }

    fputs("\nRemoving:  ", stdout);
    for (i = numData - 1; i >= 0; --i) {
        testKey = i * 1000;
        value = (long int *)map_delete(map, (void *)&testKey);
        printf(" %ld", *value);
        free(value);
    }
    putchar('\n');
}

int main() {
    MapADT map = map_create(compare_uint);
    /* unsigned int *key = (unsigned int *)malloc(sizeof(unsigned int)); */
    /* long int *value = (long int *)malloc(sizeof(long int)); */
    /* *key = 5; */
    /* *value = 69; */
    /* map_put(map, (void *)key, (void *)value); */
    /* printf("%d\n", *(unsigned int *)map_get(map, (void *)key)); */
    /* value = (long int *)map_delete(map, (void *)key); */
    /* printf("Value removed was %d\n", (int)*value); */

    map_get(NULL, (void *)"fuck");
    map_destroy(map);
    return 0;
}
