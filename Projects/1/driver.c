#include "DebugMacros.h"
#include "bracetopia.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Hello Bracetopia\n");

    BracetopiaBoard board;

    if (!(argv[0] && argc)) {
        printf("fuck\n");
    }
    createBracetopia(&board, 10, 0.7);

    for (int i = 0; i < board.size; i++) {
        printf("%p\n", (void *)board.board[i]);
    }

    destroyBracetopia(&board);
    return 0;
}
