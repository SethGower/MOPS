/******************************************************************************
* File:             hist.c
*
* Author:           Seth Gower  
* Created:          09/06/19 
* Description:      Histogram program to read in standard input and create a
*                   histogram of the number of characters
*****************************************************************************/

#include "hist.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *frequencies = calloc(HIST_SIZE, sizeof(int));
    int nn = read_text(frequencies);

    if (NULL != frequencies) {
        if (0 != nn) {
            printf("%d alphabetic characters read\n", nn);
            display_histogram(frequencies);

        } else {
            fprintf(stderr, "no alphabetic characters read\n");
        }
        free(frequencies);
        return 0;
    }
    return -1;
}

/******************************************************************************
 * Function:         int read_text
 * Description:      Reads text from STDIN and generates a frequency list in
 *                   the provided integer array
 * Where:
 *                   int *frequencies - List of letter frequencies
 * Return:           Number of characters in data set
 * Error:
 *****************************************************************************/
int read_text(int *frequencies) {
    char currChar = getchar(); /* current character function is working with */
    int charn = 0;             /* number of characters in the text */
    while (EOF != currChar) {  /* run until EOF */
        /* this if statement checks if the character is lower case */
        if (currChar >= 97 && currChar <= 122) {
            currChar -= 32; /* if lower case, make upper case */
        }
        /* if the character is an upper case, work */
        if (currChar >= 65 && currChar <= 90) {
            currChar -= 65;
            frequencies[(int)currChar]++;
            charn++;
        }
        /* sets the currChar to next character for next iteration of loop */
        currChar = getchar();
    }
    return charn;
}

/******************************************************************************
 * Function:         void display_histogram
 * Description:      Display histogram of letter frequencies
 * Where:
 *                   int* frequencies - frequency list of letters
 * Return:           void
 *****************************************************************************/
void display_histogram(int *frequencies) {
    int i = 0;
    int j = 0;
    int highest = 0;
    /* determines the highest frequency character in the set */
    for (i = 0; i < HIST_SIZE; i++) {
        if (frequencies[i] > highest) {
            highest = frequencies[i];
        }
    }

    /* nested for loop to iterate through the matrix of characters */
    /* outer loop creates the rows, where the number of rows is the highest
     * amount of characters in the set */
    for (i = highest; i > 0; i--) {
        for (j = 0; j < HIST_SIZE; j++) {
            /* if the current index is less than or equal to the frequency of
             * the current letter, then put an X, else put a space */
            if (i <= frequencies[j]) {
                putchar('X');
            } else {
                putchar(' ');
            }
        }
        /* go to the next line */
        putchar('\n');
    }
    printf("--------------------------\n");
    printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
}
