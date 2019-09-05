#include "hist.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("Hello World\n");
    int *frequencies = calloc(26, sizeof(int));
    read_text(frequencies);
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", (char)(i + 65), frequencies[i]);
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
void display_histogram(int *frequencies) {}
