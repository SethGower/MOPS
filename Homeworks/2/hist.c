#include "hist.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int *frequencies = calloc(HIST_SIZE, sizeof(int));
    int nn = read_text(frequencies);

    if (0 != nn) {
        display_histogram(frequencies);
        /* for (int i = 0; i < 26; i++) { */
        /*     printf("%c: %d\n", (char)(i + 65), frequencies[i]); */
        /* } */

    } else {
        printf("No alphabetic characters read\n");
    }
    return 0;
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
    for (i = 0; i < HIST_SIZE; i++) {
        if (frequencies[i] > highest) {
            highest = frequencies[i];
        }
    }

    for (i = highest; i > 0; i--) {
        for (j = 0; j < HIST_SIZE; j++) {
            if (i <= frequencies[j]) {
                putchar('X');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    printf("--------------------------\n");
    printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
}
