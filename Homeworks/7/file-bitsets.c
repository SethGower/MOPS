#include "file-bitsets.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
const size_t BUFSIZE = 256;
const char *set_def =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789";
void printBits(size_t const size, void const *const ptr);
int main(int argc, char *argv[]) {
    int returnCode = EXIT_SUCCESS;
    FILE *fp1, *fp2;
    uint64_t set1, set2;
    if (argc > 3 || argc < 3) {
        fprintf(stderr, "Usage: ./file-bitsets string1 string2\n");
        returnCode = EXIT_FAILURE;
    } else {
        fp1 = fopen(argv[1], "rb");
        if (NULL != fp1) {
            printf("string1:	%s	Encoding the file:	%s\n", argv[1],
                   argv[1]);
            set1 = file_set_encode(fp1);
            fclose(fp1);
        } else if (ENOENT == errno) {
            printf("string1:	%s	Encoding the string:	%s\n", argv[1],
                   argv[1]);
            set1 = set_encode(argv[1]);
        } else {
            perror(argv[1]);
            return EXIT_FAILURE;
        }

        fp2 = fopen(argv[2], "rb");
        if (NULL != fp2) {
            printf("string2:	%s	Encoding the file:	%s\n", argv[1],
                   argv[2]);
            set2 = file_set_encode(fp2);
            fclose(fp2);
        } else if (ENOENT == errno) {
            printf("string2:	%s	Encoding the string:	%s\n", argv[2],
                   argv[2]);
            set2 = set_encode(argv[2]);
        } else {
            perror(argv[2]);
            return EXIT_FAILURE;
        }
        putchar('\n');
        printf("set1:%c 0x%016lX\n", '\t', set1);
        printf("set2:%c 0x%016lX\n", '\t', set2);
        putchar('\n');
        printf("set_intersect:	0x%016lx\n", set_intersect(set1, set2));
        printf("set_union:	0x%016lx\n", set_union(set1, set2));
        putchar('\n');
        printf("set1 set_complement:	0x%016lx\n", set_complement(set1));
        printf("set2 set_complement:	0x%016lx\n", set_complement(set2));
        putchar('\n');
        printf("set_difference:	0x%016lx\n", set_difference(set1, set2));
        uint64_t diff = set_difference(set1, set2);
        /* printBits(sizeof(uint64_t), &set1); */
        /* printBits(sizeof(uint64_t), &set2); */
        /* printBits(sizeof(uint64_t), &diff); */
        printf("set_symdifference:	0x%016lx\n",
               set_symdifference(set1, set2));
        putchar('\n');
        printf("set1 cardinality:	%ld\n", set_cardinality(set1));
        printf("set2 cardinality:	%ld\n", set_cardinality(set2));
        putchar('\n');
        char *set1mems, *set2mems;
        printf("set1 members:	'%s'\n", set1mems = set_members(set1));
        printf("set2 members:	'%s'\n", set2mems = set_members(set2));
        free(set1mems);
        free(set2mems);
    }

    return returnCode;
}

/******************************************************************************
 * Function:         uint64_t file_set_encode
 * Description:      Takes in an opened file and generates a set from the
 *                   characters in the file
 * Where:
 *                   FILE *fp - File to be read in
 * Return:           uint64_t - Set from the characters
 * Error:            None
 *****************************************************************************/
uint64_t file_set_encode(FILE *fp) {
    uint64_t set = 0;
    char *buff;
    buff = (char *)calloc(sizeof(char), BUFSIZ);
    int ret;
    /* reads through the input file, until no more characters are read */
    while ((ret = fread(buff, 1, BUFSIZ, fp))) {
        /* updates the current set with the set of the current buffer of
         * characters from file */
        set = set | set_encode(buff);
    }
    free(buff);
    return set;
}

/******************************************************************************
 * Function:         uint64_t set_encode
 *                   Returns the set given a string
 * Where:
 *                   char *st - Character string to generate set for
 * Return:           uint64_t - Set of characters
 * Error:            None
 *****************************************************************************/
uint64_t set_encode(char *st) {
    uint64_t set = 0;
    size_t i = 0;
    uint64_t shift = 0;
    for (i = 0; i < strlen(st); i++) {
        shift = ((SETSIZE - 1) - get_index(set_def, st[i]));
        /* printf("Shift: %ld\n", shift); */
        /* printf("Index: %d\n", get_index(st[i])); */
        set |= (uint64_t)1 << shift;
    }
    return set;
}

/******************************************************************************
 * Function:         uint8_t get_index
 *                   Returns the index of the character in the set
 * Where:
 *                   char c - Character to be checked
 * Return:           Integer index in the set
 * Error:            Sets errno to EINVAL (invalid argument)
 *****************************************************************************/
uint8_t get_index(const char *set, char c) { return strchr(set, c) - set; }

void printBits(size_t const size, void const *const ptr) {
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}
uint64_t set_intersect(uint64_t set1, uint64_t set2) { return set1 & set2; }
uint64_t set_union(uint64_t set1, uint64_t set2) { return set1 | set2; }
uint64_t set_complement(uint64_t set1) { return ~set1; }
uint64_t set_difference(uint64_t set1, uint64_t set2) { return set2 ^ set1; }
uint64_t set_symdifference(uint64_t set1, uint64_t set2) { return 0; }

/******************************************************************************
 * Function:         size_t set_cardinality
 *                   Return the cardinality (number of elements) in the set
 * Where:
 *                   uint64_t set - Set to check
 * Return:           size_t - cardinality of set
 * Error:            None
 *****************************************************************************/
size_t set_cardinality(uint64_t set) {
    size_t count = 0;
    uint64_t shifted = 0;
    int i = 0;
    for (i = 0; i < SETSIZE; i++) {
        shifted = set >> i;
        if (shifted % 2) /* if it is even, that means lsb is 1, meaning there
                            was an item at that index */
            count++;
    }
    return count;
}

/******************************************************************************
 * Function:         char *set_members
 * Description:      Return a string (on the heap) containing all of the
 *                   characters in the set, in the order in which they appear in
 *                   the string
 * Where:
 *                   uint64_t set - set to check
 * Return:           char* - String of the contents
 * Error:            None
 *****************************************************************************/
char *set_members(uint64_t set) {
    char *set_mems = (char *)calloc(1, SETSIZE + 1);
    int i = 0;
    uint64_t shifted = 0;
    char *buff = (char *)calloc(1, 2);
    for (i = SETSIZE - 1; i >= 0; i--) {
        shifted = set >> i;
        if (shifted % 2) { /* if it is even, that means lsb is 1, meaning there
                              was an item at that index */
            buff[0] = set_def[SETSIZE - 1 - i];
            strcat(set_mems, buff);
        }
    }
    free(buff);
    return set_mems;
}
