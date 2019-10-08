#include "DebugMacros.h"
#include "bracetopia.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Hello Bracetopia\n");

    BracetopiaBoard board;

    if (!(argv[0] && argc)) {
        printf("fuck\n");
    }
    createBracetopia(&board, 10, 0.5);

    populateBoard(&board, 0.4, 0.6);
    printGrid(&board);
    printf("%f\n", board.happiness);
    move(&board);
    printGrid(&board);
    printf("%f\n", board.happiness);

    destroyBracetopia(&board);
    return 0;
}
