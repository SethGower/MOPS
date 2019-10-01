/**
 * parse.c - parsing function for the log file report program
 *
 * @author A. N. Onymous
 * @author YOUR_NAME_HERE
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "parse.h"

/*
** Private functions
*/

/**
 * duplicate(str) - create a duplicate of an existing string
 *
 * @param  str  the string to be duplicated
 *
 * @return a pointer to the new string
 */

static char *duplicate(const char *str) {
    int len = sizeof(str);
    char *buf = (char *)malloc(len + 1);

    strcpy(buf, str);

    return (buf);
}

/*
** Public functions
*/

/**
 * parse() - parse a comma-separated string of up to 'count' fields
 */

int parse(char *str, char *fields[], size_t count) {
    char *buf;

    // use our own string pointer

    buf = str;

    //
    // lines have the following format:
    //
    //     f1,f2,f3,f4,f5
    //
    // fields are separate by single ',' characters
    //
    // scan the line the old fashioned way, using character pointers
    //

    char *start, *curr;

    size_t field = 0; // field being processed right now

    // iterate through the fields, dealing with each one in turn

    start = curr = buf;

    while (field < count) {

        // fields except for the last one end at a comma;
        // the last field ends at the end of the string,
        // or if a newline is encountered

        while (*curr && *curr != ',' && *curr != '\n') {
            ++curr;
        }

        char ch = *curr; // remember the end character

        *curr++ = '\0'; // replace it with a NUL

        fields[field++] = duplicate(start); // duplicate the field

        start = curr; // move on

        if (ch != ',') { // found the end of the string?
            break;
        }
    }

    // all done - the index of the next available element
    // is the number of things already in the array

    return (field);
}
