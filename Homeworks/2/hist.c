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

int read_text(int *frequencies) {
    char currChar = getchar();
    int charn = 0;
    while (EOF != currChar) {
        if (currChar >= 97 && currChar <= 122) {
            currChar -= 32;
        }
        if (currChar >= 65 && currChar <= 90) {
            currChar -= 65;
            frequencies[(int)currChar]++;
        }
        currChar = getchar();
    }
    return charn;
}

void display_histogram(int *frequencies) {}
