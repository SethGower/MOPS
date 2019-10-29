///
/// Description:  test driver for the MapADT module
///
/// This test program creates a map using string keys and
/// performs a number of operations inserting and removing
/// long int values.
///
/// @author ckd
/// @author wrc
///

// need setvbuf() declaration
#define _DEFAULT_SOURCE

#include "mapADT.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

///
/// Sequence of test values to be inserted and removed
///
long int testData[] = {42, 17, -12, 9982, 476, 2912, -22, 3291213, 7782};
int numData = sizeof(testData) / sizeof(long int);

///
/// Comparison function for C string keys
///
/// @param a first value to compare
/// @param b second value to compare
/// @return integer indicating the relationship between a and b
///
int compare_str(const void *a, const void *b) {
    char *key1, *key2;

    key1 = (char *)a;
    key2 = (char *)b;

    return (strcmp(key1, key2));
}

///
/// Process insertions/removals using a specific map
///
/// @param map the MapADT to use for the tests
///
void process(MapADT map) {
    int i;
    char *key;
    long int *value;
    char *keys[] = {"apple", "banana", "cat",   "dog",  "red",
                    "blue",  "green",  "water", "table"};

    // load data into the map

    fputs("Filling:\n", stdout);
    for (i = 0; i < numData; ++i) {
        key = (char *)malloc(strlen(keys[i]) + 1);
        assert(NULL != key);
        strncpy(key, keys[i], strlen(keys[i]) + 1);
        value = (long int *)malloc(sizeof(long int));
        assert(NULL != value);
        *value = testData[i];
        printf("[%s, %ld] ", key, *value);
        map_put(map, (void *)key, (void *)value);
    }

    // begin the tests

    fputs("\n\nRetrieving in reverse order:\n", stdout);
    for (i = numData - 1; i >= 0; --i) {
        key = keys[i];
        value = (long int *)map_get(map, (void *)key);
        printf("[%s, %ld] ", key, *value);
    }

    fputs("\n\nClearing map\n\n", stdout);
    map_clear(map);

    fputs("Refilling:\n", stdout);
    for (i = 0; i < numData; ++i) {
        key = (char *)malloc(strlen(keys[i]) + 1);
        assert(NULL != key);
        strncpy(key, keys[i], strlen(keys[i]) + 1);
        value = (long int *)malloc(sizeof(long int));
        assert(NULL != value);
        *value = testData[i];
        printf("[%s, %ld] ", key, *value);
        map_put(map, (void *)key, (void *)value);
    }

    fputs("\n\nPutting apple\n", stdout); // again
    key = (char *)malloc(strlen("apple") + 1);
    strncpy(key, "apple", strlen("apple") + 1);
    value = (long int *)malloc(sizeof(long int));
    *value = 1;
    value = map_put(map, (void *)key, (void *)value);
    assert(NULL != value);
    printf("Old value of apple: %ld\n", *value);
    free(key);
    free(value);

    fputs("\nPutting apple\n", stdout); // again
    key = (char *)malloc(strlen("apple") + 1);
    strncpy(key, "apple", strlen("apple") + 1);
    value = (long int *)malloc(sizeof(long int));
    *value = 250;
    value = map_put(map, (void *)key, (void *)value);
    assert(NULL != value);
    printf("Old value of apple: %ld\n", *value);
    free(key);
    free(value);

    fputs("\nPutting cat\n", stdout); // again
    key = (char *)malloc(strlen("cat") + 1);
    strncpy(key, "cat", strlen("cat") + 1);
    value = (long int *)malloc(sizeof(long int));
    *value = 255;
    value = map_put(map, (void *)key, (void *)value);
    assert(NULL != value);
    printf("Old value of cat: %ld\n", *value);
    free(key);
    free(value);

    fputs("\nPutting chair\n", stdout);
    key = (char *)malloc(strlen("chair") + 1);
    strncpy(key, "chair", strlen("chair") + 1);
    value = (long int *)malloc(sizeof(long int));
    *value = 255;
    value = map_put(map, (void *)key, (void *)value);
    assert(NULL == value);
    printf("chair was not previously in the map\n");
}

///
/// main routine - initiate all the tests
///
int main(void) {
    MapADT myMap;

    //
    // to aid in debugging, we reset stdout to be line buffered
    // so that we don't lose any output if an assertion fails
    // even if stdout is redirected into a file
    //
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    myMap = map_create(compare_str);
    if (myMap == NULL) {
        fputs("Cannot create map with C string keys\n", stderr);
        exit(EXIT_FAILURE);
    }
    puts("\nTesting the [C string, long int] map");
    process(myMap);
    map_destroy(myMap);

    return 0;
}
