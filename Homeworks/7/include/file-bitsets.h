#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SETSIZE (sizeof(uint64_t) << 3)

uint64_t file_set_encode(FILE *fp);
uint64_t set_encode(char *st);
uint8_t get_index(const char *set, char c);
uint64_t set_intersect(uint64_t set1, uint64_t set2);
uint64_t set_union(uint64_t set1, uint64_t set2);
uint64_t set_complement(uint64_t set1);
uint64_t set_difference(uint64_t set1, uint64_t set2);
uint64_t set_symdifference(uint64_t set1, uint64_t set2);
size_t set_cardinality(uint64_t set);
char *set_members(uint64_t set); // the string of characters in the bit set
