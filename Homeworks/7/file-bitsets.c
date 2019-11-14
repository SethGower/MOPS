#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SETSIZE (sizeof(uint64_t) << 3)
#define BUFSIZE 256

/* I hate how we weren't allowed to have a header file... */
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
const char *set_def =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789";
int main(int argc, char *argv[]) {
    int returnCode = EXIT_SUCCESS;
    FILE *fp1, *fp2;
    uint64_t set1, set2;
    if (argc > 3 || argc < 3) {
        fprintf(stderr, "Usage: %s string1 string2\n", argv[0]);
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
            printf("string2:	%s	Encoding the file:	%s\n", argv[2],
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
        printf("set1:	0x%016lx\n", set1);
        printf("set2:	0x%016lx\n", set2);
        putchar('\n');
        printf("set_intersect:	0x%016lx\n", set_intersect(set1, set2));
        printf("set_union:	0x%016lx\n", set_union(set1, set2));
        putchar('\n');
        printf("set1 set_complement:	0x%016lx\n", set_complement(set1));
        printf("set2 set_complement:	0x%016lx\n", set_complement(set2));
        putchar('\n');
        printf("set_difference:		0x%016lx\n", set_difference(set1, set2));
        printf("set_symdifference:	0x%016lx\n",
               set_symdifference(set1, set2));
        putchar('\n');
        printf("set1 set_cardinality:	%ld\n", set_cardinality(set1));
        printf("set2 set_cardinality:	%ld\n", set_cardinality(set2));
        putchar('\n');
        char *set1mems, *set2mems;
        printf("members of set1:	'%s'\n", set1mems = set_members(set1));
        printf("members of set2:	'%s'\n", set2mems = set_members(set2));
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
    buff = (char *)calloc(sizeof(char), BUFSIZE + 1);
    int ret = 0;
    /* reads through the input file, until no more characters are read */
    while ((ret = fread(buff, 1, BUFSIZE, fp))) {
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
    uint8_t index = 0;
    for (i = 0; i < strlen(st); i++) {
        index = get_index(set_def, st[i]);
        if (SETSIZE <= index)
            continue;
        shift = ((SETSIZE - 1) - index);
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
uint8_t get_index(const char *set, char c) {
    /* strchr returns the pointer to the character being searched for in the
     * string. So the index is the difference between the characters pointer
     * and the given set being seached's pointer */
    return strchr(set, c) - set;
}

/******************************************************************************
 * Function:         uint64_t set_intersect
 * Description:      Returns the intersect of the two sets. The intersect is
 *                   defined as the elements that reside in both sets, ie an
 *                   AND operation
 * Where:
 *                   uint64_t set1 - the first set argument
 *                   uint64_t set2 - the second set argument
 * Return:           The set (subset) that is the intersect of the two sets
 * Error:            None
 *****************************************************************************/
uint64_t set_intersect(uint64_t set1, uint64_t set2) {
    /* returns the intersect, which is just an AND operation */
    return set1 & set2;
}

/******************************************************************************
 * Function:         uint64_t set_union
 * Description:      Returns the union of the two sets, which is defined as the
 *                   superset that contains all of the elements in either set,
 *                   ie an OR operation
 * Where:
 *                   uint64_t set1 - the first set argument
 *                   uint64_t set2 - the second set argument
 * Return:           The set containing all elements in either set
 * Error:            None
 *****************************************************************************/
uint64_t set_union(uint64_t set1, uint64_t set2) {
    /* the union is just the OR operation */
    return set1 | set2;
}

/******************************************************************************
 * Function:         uint64_t set_complement
 * Description:      Returns the complement of the set. This is defined as the
 *                   set that contains all elements not in the given set, ie a
 *                   NOT operation
 * Where:
 *                   uint64_t set1 - the first set argument
 * Return:           The complement of the set
 * Error:            None
 *****************************************************************************/
uint64_t set_complement(uint64_t set1) {
    /* the complement is just the inverted set */
    return ~set1;
}

/******************************************************************************
 * Function:         uint64_t set_difference
 * Description:      Returns the difference of the two sets. This is defined as
 * the set that contains all of the elements in the first set, that were not in
 *                   Returns the difference of the two sets. This is defined as
 *                   * the set that contains all of the elements in the first
 *                   set, that were not in * the second set
 * Where:
 *                   uint64_t set1 - the first set argument
 *                   uint64_t set2 - the second set argument
 * Return:           Returns the set: set1-set2
 * Error:            None
 *****************************************************************************/
uint64_t set_difference(uint64_t set1, uint64_t set2) {
    uint64_t diff = 0;     /* current running result set */
    long int i = 0;        /* loop control variable */
    uint64_t shifted1 = 0; /* values to store shfited versions of sets */
    uint64_t shifted2 = 0;
    /* loop through every single bit in the set */
    for (i = SETSIZE - 1; i >= 0; i--) {
        shifted1 =
            set1 >> i; /* creates the shifted sets to get the current value */
        shifted2 = set2 >> i;
        /* checks if even, since that means the current bit being checked is 0
         * if this is false (lsb == 1) and the lsb of set2 (shifted) is 0,
         * then shift a 1 into the current place and OR it with the current
         * running result set */
        if (shifted1 % 2 && !(shifted2 % 2)) {
            diff |= ((uint64_t)1 << (i));
        }
    }
    return diff;
}

/******************************************************************************
 * Function:         uint64_t set_symdifference
 * Description:      Returns the symmetric difference of the two sets. This is
 *                   defined as: set_union(set_difference(set1,set2),
 *                   set_difference(set2, set1))
 * Where:
 *                   uint64_t set1 - the first set argument
 *                   uint64_t set2 - the second set argument
 * Return:           The symmetric difference
 * Error:            None
 *****************************************************************************/
uint64_t set_symdifference(uint64_t set1, uint64_t set2) {
    uint64_t diff1, diff2;
    diff1 = set_difference(set1, set2);
    diff2 = set_difference(set2, set1);
    return set_union(diff1, diff2);
}

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
    size_t i = 0;
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
